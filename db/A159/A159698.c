/*
 * A159698  Minimal increasing sequence beginning with 4 such that n and a(n)
 *          are either both prime or both nonprime.
 * https://oeis.org/A159698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = minimum m > a(n-1) such that m is prime iff n is prime.
 *            a(1) = 4. The sequence is memoised; each term is computed once
 *            on demand. The implementation uses the supplied prime‑test
 *            helper functions and a small dynamic allocator (grow_seq) that
 *            works correctly for both built‑in and arbitrary‑p...
 * Range:     u8 n <= 150, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 86 terms through n = 8220 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"
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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Return the n‑th term of sequence A159698 (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence is defined only for n >= 1. */
        return (seq_t)0;
    }

    /* Static cache of computed terms.  cache[0] holds a(1). */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* allocated size */
    static idx_t  cache_n    = 0;   /* number of terms stored */

    /* Grow the cache so that it can hold at least n elements. */
    if (cache_cap < n) {
        grow_seq(&cache, &cache_cap, n);
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cache_n; i < n; ++i) {
        if (i == 0) {
            /* a(1) = 4 */
            cache[i] = (seq_t)4;
            continue;
        }

        seq_t prev   = cache[i - 1];
        seq_t cand   = prev + (seq_t)1;
        bool want_prime = is_prime((idx_t)(i + 1));  /* i+1 == current n */

        /* Find the next integer with the required prime property. */
        for (;;) {
            bool cand_prime = is_prime_big(cand);
            if (cand_prime == want_prime) {
                cache[i] = cand;
                break;
            }
            cand = cand + (seq_t)1;
        }
    }

    cache_n = n;
    return cache[n - 1];
}
