/*
 * A050368  Number of ways to factor n into distinct factors with 2 kinds of
 *          2, 3 kinds of 3, ...
 * https://oeis.org/A050368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ over factorizations n = Π f^{e_f} (0 ≤ e_f ≤ f) of Π
 *            binom(f, e_f)
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where binom(f, e_f) is the binomial coefficient C(f, e_f) */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/binom_uint.h"

/* Cache for already computed terms */
static seq_t *a_cache = NULL;
static size_t cache_cap = 0;

/* Ensure the cache can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        a_cache = tmp;
        for (size_t i = cache_cap; i < new_cap; ++i)
            a_cache[i] = 0;          /* 0 means "not computed" */
        cache_cap = new_cap;
    }
}

/* Recursive enumeration of unordered factorizations */
static void dfs(uint64_t rem, uint64_t min_f, seq_t weight, seq_t *out)
{
    if (rem == 1) {
        *out += weight;
        return;
    }

    /* Iterate over divisors d of rem, d >= min_f, d*d <= rem */
    for (uint64_t d = min_f; d * d <= rem; ++d) {
        if (rem % d != 0)
            continue;

        /* Process divisor d with exponent k (1..d, d^k divides rem) */
        uint64_t pow = d;
        for (uint64_t k = 1; k <= d && rem % pow == 0; ++k) {
            seq_t bin = binom_uint(d, k);
            seq_t new_weight = ((unsigned __int128)weight * bin);
            dfs(rem / pow, d + 1, new_weight, out);

            if (pow > rem / d)  /* next power would overflow or exceed rem */
                break;
            pow *= d;
        }

        /* Process complementary divisor d2 = rem/d, if distinct and ≥ min_f */
        uint64_t d2 = rem / d;
        if (d2 != d && d2 >= min_f) {
            pow = d2;
            for (uint64_t k = 1; k <= d2 && rem % pow == 0; ++k) {
                seq_t bin = binom_uint(d2, k);
                seq_t new_weight = ((unsigned __int128)weight * bin);
                dfs(rem / pow, d2 + 1, new_weight, out);

                if (pow > rem / d2)
                    break;
                pow *= d2;
            }
        }
    }

    /* Handle the remaining factor being the whole rem itself */
    if (rem >= min_f) {
        seq_t bin = binom_uint(rem, 1);          /* binom(rem,1) = rem */
        seq_t new_weight = ((unsigned __int128)weight * bin);
        dfs(1, rem + 1, new_weight, out);
    }
}

/* Public entry point for the sequence */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    ensure_cache(n);
    if (a_cache[n] != 0)
        return a_cache[n];

    seq_t result = 0;
    dfs((uint64_t)n, 2, (seq_t)1, &result);
    a_cache[n] = result;
    return result;
}
