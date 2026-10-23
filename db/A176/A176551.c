/*
 * A176551  Products of 2 primes of the form 3*k-+1.
 * https://oeis.org/A176551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   sieve_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Products of two primes not equal to 3, sorted ascending. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/sieve_uint32.h"

/* static storage */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static uint32_t *prime_array = NULL;
static size_t prime_cnt = 0;
static idx_t prime_limit = 0;

/* Compare function for qsort */
static int seq_cmp(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    return (*x < *y) ? -1 : (*x > *y);
}

/* Ensure we have at least n terms precomputed */
static void ensure_terms(idx_t n)
{
    if (n == (idx_t)0) return;
    if (terms_len >= (size_t)n) return;

    /* limit flag: true if seq_t is a built‑in unsigned type (size <= 8). */
    const int limit_flag = sizeof(seq_t) <= sizeof(unsigned long long);
    const seq_t max_seq_limit = limit_flag ? (seq_t)-1 : (seq_t)0;

    /* start with a moderate prime limit */
    if (prime_limit == 0) prime_limit = 1000;
    else prime_limit *= 2;

    /* generate primes up to prime_limit */
    uint32_t *new_primes = NULL;
    size_t new_cnt = 0;
    sieve_uint32(prime_limit, &new_primes, &new_cnt);
    if (!new_primes) {
        /* cannot proceed */
        return;
    }
    if (prime_array) free(prime_array);
    prime_array = new_primes;
    prime_cnt = new_cnt;

    /* estimate capacity for product array */
    size_t capacity = prime_cnt * (prime_cnt + 1) / 2;
    seq_t *prod_arr = (seq_t *)malloc(capacity * sizeof(seq_t));
    if (!prod_arr) return;

    size_t prod_len = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t pi = prime_array[i];
        if (pi % 3 == 0) continue;   /* skip primes equal to 3 */
        for (size_t j = i; j < prime_cnt; ++j) {
            uint32_t pj = prime_array[j];
            if (pj % 3 == 0) continue;
            __uint128_t mul = (__uint128_t)pi * pj;
            if (limit_flag && mul > (__uint128_t)max_seq_limit) continue;
            prod_arr[prod_len++] = (seq_t)mul;
        }
    }

    /* sort and deduplicate */
    qsort(prod_arr, prod_len, sizeof(seq_t), seq_cmp);
    size_t uniq_len = 0;
    for (size_t i = 0; i < prod_len; ++i) {
        if (i == 0 || prod_arr[i] != prod_arr[i - 1]) {
            prod_arr[uniq_len++] = prod_arr[i];
        }
    }

    if (terms) free(terms);
    terms = prod_arr;
    terms_len = uniq_len;

    /* if still not enough, recurse with larger prime limit */
    if (terms_len < (size_t)n) {
        free(terms);
        terms = NULL;
        terms_len = 0;
        ensure_terms(n);
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;
    ensure_terms(n);
    return terms[(size_t)n - 1];
}
