/*
 * A132251  Isolated primes congruent to 17 (mod 30).
 * https://oeis.org/A132251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates OEIS sequence A132251.
   It searches for primes congruent to 17 (mod 30) that are not part of a
   twin‑prime pair (i.e., both p–2 and p+2 are composite).  The algorithm
   works by iterating over the arithmetic progression 30·k+17 for k ≥ 1,
   testing each candidate for primality and for the absence of twin primes.
   The results are memoised in a static array so that calls in any order
   are O(1) after the first discovery. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"
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

static seq_t *cache = NULL;          /* memoised terms */
static size_t cache_cap = 0;
static idx_t  cache_n   = 0;         /* number of terms stored */
static idx_t  cand_k    = 1;         /* next k value for candidate 30·k+17 */

/* Ensure the cache can hold at least `need` elements. */
static void ensure_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = (cache_cap == 0 ? 64 : cache_cap << 1);
    while (new_cap < need) new_cap <<= 1;
    /* cast the void* returned by realloc to seq_t* for C++ compatibility */
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th isolated prime ≡ 17 (mod 30) (1‑based index). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n == 0, defensive guard */

    while (cache_n < n) {
        seq_t cand = ((seq_t)30 * cand_k) + 17;

        if (is_prime_big(cand)) {
            /* candidate is prime; check that it is not in a twin pair */
            seq_t cand_minus_2 = cand - 2;
            seq_t cand_plus_2  = cand + 2;
            if (!is_prime_big(cand_minus_2) && !is_prime_big(cand_plus_2)) {
                ensure_cap(cache_n + 1);
                cache[cache_n++] = cand;
            }
        }
        cand_k++;   /* move to the next potential candidate */
    }
    return cache[n - 1];
}
