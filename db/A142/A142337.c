/*
 * A142337  Numbers m such that Sum_{i=1..m} sigma(i) is prime.
 * https://oeis.org/A142337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 28, u32 n <= 4222, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_sqrt, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers m such that Sum_{i=1..m} sigma(i) is prime.
   The function computes the sequence by incremental search,
   storing found m values in a memoised list. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/int_sqrt.h"

/* static state for memoisation */
static idx_t last_i = 0;                /* largest m processed so far   */
static seq_t cumulative_sum = 0;        /* sum_{i=1}^{last_i} sigma(i) */

/* smallest prime factor for every i up to last_i (0 and 1 are handled separately) */
static idx_t *spf = NULL;
static size_t spf_cap = 0;

/* list of primes discovered while extending spf */
static idx_t *primes = NULL;
static size_t primes_cap = 0;
static size_t primes_cnt = 0;

/* list of m where cumulative_sigma is prime */
static idx_t *prime_m_list = NULL;
static size_t prime_list_cap = 0;
static size_t prime_count = 0;

/*------------------ helper functions ------------------*/

/* Compute sigma(i) using the spf array. */
static seq_t sigma_of(idx_t n)
{
    if (n == 0) return 0;
    seq_t result = 1;
    while (n > 1) {
        idx_t p = spf[n];
        seq_t pe = 1;
        seq_t sum_p = 1;
        while (n % p == 0) {
            n /= p;
            pe *= (seq_t)p;
            sum_p += pe;
        }
        result *= sum_p;
    }
    return result;
}

/* Append val to a dynamic array of idx_t, growing if necessary. */
static void grow_idx_arr(idx_t **arr, size_t *cap, size_t *cnt, idx_t val)
{
    if (*cnt >= *cap) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        *arr = (idx_t *)realloc(*arr, new_cap * sizeof(idx_t));
        *cap = new_cap;
    }
    (*arr)[*cnt] = val;
    (*cnt)++;
}

/* Extend the spf array, primes list and last_i by one element. */
static void extend_one(void)
{
    idx_t i = last_i + 1;

    /* Resize spf array to accommodate i */
    spf = (idx_t *)realloc(spf, (i + 1) * sizeof(idx_t));
    spf_cap = i + 1;

    /* Initialise spf[0] and spf[1] on first call */
    if (i == 0) {
        spf[0] = 0;
        return;
    }
    if (i == 1) {
        spf[1] = 1;
        last_i = 1;
        return;
    }

    /* Find smallest prime factor of i using known primes */
    idx_t p_factor = 0;
    idx_t sqrt_i = int_sqrt(i);
    for (size_t k = 0; k < primes_cnt && primes[k] <= sqrt_i; ++k) {
        if (i % primes[k] == 0) {
            p_factor = primes[k];
            break;
        }
    }
    if (p_factor == 0) {       /* i is prime */
        p_factor = i;
        grow_idx_arr(&primes, &primes_cap, &primes_cnt, i);
    }
    spf[i] = p_factor;
    last_i = i;
}

/* Ensure that we have processed enough terms to contain the target number
   of records in prime_m_list. */
static void generate_until(idx_t target)
{
    while (prime_count < target) {
        extend_one();                       /* new i = last_i           */
        seq_t sigma = sigma_of(last_i);
        cumulative_sum += sigma;
        if (isPrimeGeneric(cumulative_sum)) {
            grow_idx_arr(&prime_m_list, &prime_list_cap,
                         &prime_count, last_i);
        }
    }
}

/*-------------------------------------------------------*/

/* External function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 0   */
    generate_until(n);
    return (seq_t)prime_m_list[n - 1];
}
