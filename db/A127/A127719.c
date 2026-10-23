/*
 * A127719  Floor of square root of sum of squares of n consecutive primes.
 * https://oeis.org/A127719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊√(∑_{i=1}^{n} p_i²)⌋ where p_i is the ith prime
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Utility to compute the floor of the integer square root. */
#include "../utils/floor_sqrt.h"

/* Utility that returns an upper bound for the nth prime. */
#include "../utils/_prime_limit_for_idx.h"

/* Utility that builds all primes ≤ limit. */
#include "../utils/build_primes_upto.h"

/* seq_t and idx_t are provided by the build system. */

seq_t a(idx_t n)
{
    /* Avoid wrong behaviour for n < 1 – the sequence is defined
       for n ≥ 1, but we guard it for robustness. */
    if (n < 1) return 0;

    /* ---------- Persistent global state ---------- */
    static uint64_t *primes          = NULL;   /* list of primes (uint64_t) */
    static size_t     primes_cnt     = 0;      /* number of primes stored   */
    static idx_t      primes_limit   = 0;      /* sieve upper bound used   */

    static seq_t *sum_sq_prefix   = NULL;      /* prefix sums of squares   */
    static size_t sum_sq_capacity = 0;         /* allocated size of array  */
    static size_t sum_sq_cnt      = 0;         /* number of prefix entries */

    /* ---------- Ensure we have at least n primes ---------- */
    if (primes_cnt < (size_t)n) {
        /* Compute a safe upper bound that contains the nth prime.
           _prime_limit_for_idx is a tested approximation.  */
        idx_t new_limit = _prime_limit_for_idx(n);

        if (new_limit > primes_limit) {
            /* Free any previously allocated prime array */
            if (primes) free(primes);

            /* Build primes up to new_limit.  The function allocates
               the array and sets *cnt_out to the count. */
            size_t new_cnt = 0;
            build_primes_upto(new_limit, &primes, &new_cnt);

            primes_cnt   = new_cnt;
            primes_limit = new_limit;
        }
    }

    /* ---------- Grow prefix‑sum array to n entries ---------- */
    if (sum_sq_cnt < (size_t)n) {
        if (sum_sq_capacity < (size_t)n) {
            size_t new_cap = sum_sq_capacity ? sum_sq_capacity << 1 : n + 1;
            seq_t *tmp = (seq_t *)realloc(sum_sq_prefix, new_cap * sizeof(seq_t));
            /* If realloc fails, we keep the old pointer – the function
               will be called again after the next call and will retry. */
            if (tmp) {
                sum_sq_prefix = tmp;
                sum_sq_capacity = new_cap;
            }
        }

        /* Compute new prefix sums.  We assume the array is still
           valid after realloc.  The first prime is primes[0]. */
        for (size_t i = sum_sq_cnt; i < (size_t)n; ++i) {
            seq_t p = (seq_t)primes[i];
            seq_t p2 = p * p;            /* square of the prime          */
            if (i == 0)
                sum_sq_prefix[i] = p2;
            else
                sum_sq_prefix[i] = sum_sq_prefix[i-1] + p2;
        }
        sum_sq_cnt = (size_t)n;
    }

    /* ---------- Return floor(sqrt(sum of squares)) ----------
       for the first n primes.  The index sum_sq_prefix[n-1]
       contains the required sum.  The floor_sqrt utility is
       available for all built‑in and arbitrary‑precision types. */
    return floor_sqrt(sum_sq_prefix[n-1]);
}
