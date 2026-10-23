/*
 * A106771  Primes with digit sum = 37.
 * https://oeis.org/A106771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 13, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes whose digit sum equals 37 by incremental search. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Compute the decimal digit sum of a 64‑bit unsigned integer. */
static inline uint64_t digit_sum_uint64(uint64_t v)
{
    uint64_t sum = 0;
    while (v > 0) {
        sum += v % 10;
        v /= 10;
    }
    return sum;
}

/* Dynamic array of the special primes found so far. */
static seq_t *special_list = NULL;
static idx_t special_cnt  = 0;
static idx_t special_cap  = 0;

/* The next odd integer candidate to test for primality. */
static uint64_t next_candidate = 3;

/* Ensure that the list can hold at least 'need' elements. */
static void ensure_special_cap(idx_t need)
{
    if (special_cap < need) {
        if (special_cap == 0) special_cap = 16;
        while (special_cap < need) special_cap <<= 1;
        special_list = (seq_t *)realloc(special_list,
                                         special_cap * sizeof(seq_t));
    }
}

/* Grow the list until it holds at least 'need' special primes. */
static void grow_sequence(idx_t need)
{
    while (special_cnt < need) {
        uint64_t cand = next_candidate;
        if (is_prime(cand) && digit_sum_uint64(cand) == 37) {
            ensure_special_cap(special_cnt + 1);
            special_list[special_cnt++] = (seq_t)cand;
        }
        next_candidate += 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* Undefined for offset 1, harmless */
    if (special_cnt < n) {
        grow_sequence(n);
    }
    return special_list[n - 1];
}
