/*
 * A224535  Odd numbers that are the sum of three distinct prime numbers.
 * https://oeis.org/A224535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search for the odd numbers that are the sum of three distinct
 *            primes
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t first_odd_index = 0;

static void ensure_primes(void)
{
    if (primes != NULL)
        return;
    /* Build a generous bound for primes. For n ≤ 10 000 the required odd
       numbers are safely below 20 000, so 1 000 000 is ample. */
    uint64_t limit = 1000000ULL;
    build_primes_upto(limit, &primes, &primes_cnt);
    while (first_odd_index < primes_cnt && primes[first_odd_index] == 2)
        ++first_odd_index;
}

/* Check whether n can be written as the sum of three distinct primes. */
static bool can_sum_three_distinct(seq_t n)
{
    ensure_primes();
    for (size_t pi = first_odd_index; pi < primes_cnt; ++pi) {
        seq_t p = (seq_t)primes[pi];
        if (p >= n)
            break;
        seq_t target = n - p;
        size_t l = first_odd_index;
        size_t r = primes_cnt - 1;
        while (l < r) {
            seq_t q = (seq_t)primes[l];
            seq_t rprime = (seq_t)primes[r];
            seq_t sum = q + rprime;
            if (sum == target) {
                if (q != p && rprime != p)
                    return true;
                if (q == p)
                    ++l;
                else
                    --r;
            } else if (sum < target) {
                ++l;
            } else {
                --r;
            }
        }
    }
    return false;
}

static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static seq_t last_odd_tested = (seq_t)13; /* will become 15 on first loop */

static void extend_terms_to(size_t goal)
{
    while (terms_len < goal) {
        last_odd_tested = last_odd_tested + 2;
        if (can_sum_three_distinct(last_odd_tested)) {
            if (terms_cap == 0) {
                terms_cap = 16;
                terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
            } else if (terms_len == terms_cap) {
                terms_cap *= 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = last_odd_tested;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;
    extend_terms_to((size_t)n);
    return terms[(size_t)(n - (idx_t)1)];
}
