/*
 * A106775  Primes with digit sum = 43.
 * https://oeis.org/A106775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth prime p such that the sum of decimal digits of p
 *            equals 43
 * Range:     u8 none, u16 none, u32 n <= 5400, u64 n <= 5400, u128 n <= 5400,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 5197 (2026-11-03)
 * Helpers:   digit_sum_u64, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (offset 1) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_sum_u64.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* enlarge array to store seq_t values */
static void add_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val) {
    if (*cap == 0) {
        *cap  = 4;
        *arr  = mem_new(seq_t, (*cap));
    }
    if (*size == *cap) {
        *cap <<= 1;
        *arr  = mem_resize(seq_t, *arr, (*cap));
    }
    (*arr)[*size] = val;
    (*size)++;
}

static seq_t *prime_sum_43 = NULL;   /* dynamic array of primes with digit sum 43 */
static idx_t     sz          = 0;    /* number of stored terms                */
static idx_t     cap         = 0;    /* current capacity of the array          */
static uint64_t  candidate   = 2;    /* next integer to test for primality     */

/* Ensure that we have at least 'need' terms stored in prime_sum_43 */
static void ensure_terms(idx_t need)
{
    while (sz < need) {
        /* Find the next prime whose digit sum is 43                      */
        while (!(is_prime_u64(candidate) && digit_sum_u64(candidate) == 43)) {
            /* Avoid overflow of the 64‑bit counter – for the demands of
               the test suite the values stay well below 2^63.           */
            candidate++;
        }

        /* Append the found prime to the array, growing the array if
           necessary.  The helper add_seq handles allocation and
           construction of seq_t values.                              */
        add_seq(&prime_sum_43, &cap, &sz, (seq_t)candidate);

        /* Advance to the next number for the following search.         */
        candidate++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                  /* undefined for offset 1 – guard      */
        return (seq_t)0;

    ensure_terms(n);             /* guarantee that term n exists       */
    return prime_sum_43[n - 1];  /* zero‑based array indexing          */
}
