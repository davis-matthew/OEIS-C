/*
 * A059466  Numbers which are the sum of their proper divisors containing the
 *          digit 7.
 * https://oeis.org/A059466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   has_digit7_uint64, resize_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers which are the sum of their proper divisors containing the digit 7.
   Implementation: sieve-like accumulation of divisors that contain the digit 7 and
   collect all numbers n with sum == n. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/has_digit7_uint64.h"
#include "../utils/resize_array.h"

/* Global state: sums of divisors and stored sequence values. */
static size_t current_limit   = 0;         /* current maximum n processed   */
static seq_t  *sum_arr        = NULL;      /* sum of proper divisors containing 7 */
static size_t sum_arr_cap     = 0;         /* capacity of sum_arr            */

static seq_t  *results_arr    = NULL;      /* array of found numbers          */
static size_t results_cnt     = 0;         /* number of terms stored          */
static size_t results_cap     = 0;         /* capacity of results_arr         */

/* Recompute sums up to new_limit and store all qualifying numbers. */
static void extend_to(size_t new_limit)
{
    if (new_limit <= current_limit) return;

    /* Allocate or resize sum array. */
    sum_arr = (seq_t *)resize_array(sum_arr, sum_arr_cap,
                                    new_limit + 1, sizeof(seq_t));
    sum_arr_cap = new_limit + 1;
    /* Initialise to zero. */
    memset((void *)sum_arr, 0, (new_limit + 1) * sizeof(seq_t));

    /* Sieve: for every divisor d with digit 7, add d to all multiples > d. */
    for (size_t d = 2; d <= new_limit; ++d) {
        if (!has_digit7_uint64(d)) continue;
        for (size_t m = d * 2; m <= new_limit; m += d) {
            sum_arr[m] += (seq_t)d;
        }
    }

    /* Build the result list.  Reallocate as needed. */
    free(results_arr);
    results_arr = NULL;
    results_cnt = 0;
    results_cap = new_limit / 5 + 10;          /* rough over‑estimate */
    results_arr = (seq_t *)malloc(results_cap * sizeof(seq_t));
    if (!results_arr) abort();

    for (size_t n = 2; n <= new_limit; ++n) {
        if (sum_arr[n] == (seq_t)n) {
            if (results_cnt == results_cap) {
                results_cap *= 2;
                results_arr = (seq_t *)resize_array(results_arr, results_cap / 2,
                                                    results_cap, sizeof(seq_t));
            }
            results_arr[results_cnt++] = (seq_t)n;
        }
    }

    current_limit = new_limit;
}

/* Ensure that at least target terms are available. */
static void ensure_n_terms(idx_t target)
{
    while (results_cnt < target) {
        size_t new_limit = current_limit == 0 ? 1000 : current_limit * 2;
        if (new_limit < (size_t)target * 2) new_limit = (size_t)target * 2;
        extend_to(new_limit);
    }
}

/* Required OEIS entry.  Works for any sequence of unsigned integers. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined but harmless */
    ensure_n_terms(n);
    return results_arr[n - 1];
}
