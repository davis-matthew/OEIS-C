/*
 * A124792  Numbers k such that k and A001414(k) are 3-almost primes.
 * https://oeis.org/A124792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 4160, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A124792: numbers k such that k and the sum of its
 * prime factors (with multiplicity) are both 3‑almost primes.
 * The function uses a one‑time sieve of smallest prime factors,
 * precomputes the count of prime factors and the sum of prime factors
 * for all integers up to 3×maximum k tested, and then builds the
 * sequence by scanning integers in increasing order. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_spf.h"   /* prototypes for build_spf */

static idx_t      current_k = 2;           /* next k to test          */
static idx_t      limit_k   = 0;           /* maximum k served        */
static idx_t      limit_total = 0;         /* maximum value (k or sum) */

static idx_t    *spf       = NULL;         /* smallest prime factor   */
static seq_t    *sumprime  = NULL;         /* sum of prime factors    */
static idx_t    *omega_arr = NULL;         /* count of prime factors */

static seq_t    *results   = NULL;         /* stored sequence values  */
static size_t    res_len   = 0;             /* number of results       */
static size_t    res_cap   = 0;             /* capacity of results     */

/* Re‑builds the spf, sumprime and omega arrays for a new upper bound.
 * new_limit_k is the new maximum k that will be tested. The arrays
 * are allocated for values up to 3×new_limit_k. */
static void recompute_arrays(idx_t new_limit_k)
{
    /* free previous arrays if any */
    if (spf)       free(spf);
    if (sumprime)  free(sumprime);
    if (omega_arr) free(omega_arr);

    /* allocate spf up to 3×new_limit_k   */
    idx_t new_limit_total = 3 * new_limit_k;
    build_spf(new_limit_total, &spf);

    sumprime  = (seq_t *)malloc((new_limit_total + 1) * sizeof(seq_t));
    omega_arr = (idx_t *)malloc((new_limit_total + 1) * sizeof(idx_t));

    /* initialise base cases */
    sumprime[0] = 0;   sumprime[1] = 0;
    omega_arr[0] = 0;  omega_arr[1] = 0;

    /* compute values for all integers 2 … new_limit_total */
    for (idx_t i = 2; i <= new_limit_total; ++i)
    {
        idx_t  p     = spf[i];
        idx_t  rest  = i / p;
        seq_t  rest_sum = sumprime[rest];
        seq_t  sp = (seq_t)p;        /* cast prime to seq_t */

        sumprime[i]  = rest_sum + sp;             /* sum of prime factors */
        omega_arr[i] = 1 + omega_arr[rest];       /* count of prime factors */
    }

    limit_k      = new_limit_k;
    limit_total  = new_limit_total;
}

/* Push the next qualifying k into the results array. */
static void push_result(idx_t k)
{
    if (res_len == res_cap)
    {
        size_t new_cap = res_cap == 0 ? 16 : res_cap * 2;
        seq_t *tmp = (seq_t *)realloc(results, new_cap * sizeof(seq_t));
        results = tmp;
        res_cap = new_cap;
    }
    results[res_len++] = k;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* offset is 1 */

    /* initialise arrays if not done yet */
    if (limit_k == 0)
        recompute_arrays(100000);         /* starting bound */

    /* generate results until we have at least n of them */
    while (res_len < n)
    {
        /* extend the sieve when the current k would exceed bounds */
        if (current_k > limit_k)
        {
            idx_t new_limit_k = limit_k * 2;
            if (new_limit_k < current_k * 2)
                new_limit_k = current_k * 2;   /* cover the needed k */
            recompute_arrays(new_limit_k);
        }

        idx_t k = current_k;

        /* check k */
        if (omega_arr[k] == 3)
        {
            idx_t sum_k = (idx_t)sumprime[k];
            if (sum_k <= limit_total && omega_arr[sum_k] == 3)
                push_result(k);
        }
        ++current_k;
    }

    return results[n - 1];
}
