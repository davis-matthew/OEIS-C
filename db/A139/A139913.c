/*
 * A139913  Primes of the form 17x^2+8xy+17y^2.
 * https://oeis.org/A139913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   The primes of the form 17x²+8xy+17y², which are exactly the *
 *            primes that are congruent to one of the 18 residues below modulo
 *            1092. * This implementation searches the integers in increasing
 *            order, * keeps those that satisfy a residue condition and for
 *            which is_prime_big * returns true, caching t...
 * Range:     u8 n <= 3, u16 n <= 400, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 58 terms through n = 8940 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Residues of the required primes modulo 1092. */
static const unsigned int residues[18] = {
    17, 101, 173, 185, 209, 257, 269, 341, 425,
    521, 545, 677, 797, 857, 881, 965, 1013, 1049
};

/* prototype provided by the harness */
#include <stdint.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Cache of found primes */
static seq_t *cache   = NULL;
static size_t cache_n = 0;   /* number of stored primes   */
static size_t cache_c = 0;   /* allocated capacity        */

/* The next integer to test */
static idx_t next_candidate = 16;   /* 16 so that the first test is 17 */

/* Append a prime to the cache, reallocating if necessary */
static void cache_append(seq_t p)
{
    if (cache_n >= cache_c) {
        size_t new_cap = (cache_c == 0) ? 16 : cache_c * 2;
        seq_t *new_arr = mem_resize(seq_t, cache, new_cap);
        if (!new_arr) abort();          /* out of memory */
        cache   = new_arr;
        cache_c = new_cap;
    }
    cache[cache_n++] = p;
}

/* Test whether a number modulo 1092 is one of the allowed residues. */
static inline bool ok_residue(idx_t v)
{
    idx_t r = v % (idx_t)1092;           /* use idx_t for the modulus */
    for (unsigned int i = 0; i < 18; ++i)
        if (r == residues[i]) return true;
    return false;
}

/* Extend the cache until it contains at least n primes. */
static void ensure_cache(idx_t n)
{
    while (cache_n < (size_t)n) {
        ++next_candidate;
        if (!ok_residue(next_candidate)) continue;
        seq_t cand = (seq_t)next_candidate;   /* cast for the prime test */
        if (is_prime_big(cand))
            cache_append(cand);
    }
}

/* Return the nth number in the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n = 0, but harmless */
    ensure_cache(n);
    return cache[n - 1];
}
