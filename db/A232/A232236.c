/*
 * A232236  Primes that can be written in binary representation as
 *          concatenation of two primes: p and p+2. That is, primes
 *          representable as p * 2^L + p+2, where p and p+2 are primes, and L
 *          is the length of binary representation of p+2: L = A070939(p+2).
 * https://oeis.org/A232236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = p * 2^L + (p + 2), where p and p + 2 are twin primes and
 *            L = bitlen(p + 2). This function generates those terms on demand
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 28 (2026-11-03)
 * Helpers:   bit_length, is_prime_seq, is_prime_u64, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bit_length.h"
#include "../utils/is_prime_seq.h"
#include "../utils/pow2.h"
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

/* ---------- Cached values ----------
 * terms are stored zero‑based: seq_cache[0] = a(1).  */
static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;  /* capacity of seq_cache   */
static size_t cache_len = 0;  /* number of terms stored */

/* ---------- Twin prime search ----------
 * occ_n   – the current odd candidate for p in a twin pair.  */
static idx_t occ_n = 3;

/* Append a new term to the cache, growing the array if needed. */
static void append_term(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
        if (!tmp) abort();                /* malloc failure: terminate */
        seq_cache = tmp;
        cache_cap = new_cap;
    }
    seq_cache[cache_len++] = val;
}

/* Return the nth term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)               /* offset is 1; lower indices are irrelevant */
        return 0;

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        /* Skip non‑prime candidates. */
        if (!is_prime_seq((seq_t)occ_n)) {
            occ_n += 2;
            continue;
        }
        if (!is_prime_seq((seq_t)(occ_n + 2))) {
            occ_n += 2;
            continue;
        }

        /* Bit length of the right part (p + 2). */
        idx_t L = bit_length((seq_t)(occ_n + 2));

        /* Concatenate the binary representations. */
        seq_t val = ((seq_t)occ_n) * pow2(L) + (occ_n + 2);

        /* Store the term if it is prime. */
        if (is_prime_big(val))
            append_term(val);

        occ_n += 2;
    }

    return seq_cache[n - 1];
}
