/*
 * A076385  Numbers n such that sum of digits in base 7 is a divisor of sum of
 *          prime divisors (A008472).
 * https://oeis.org/A076385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   sum_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes by brute‑force search of integers until n terms are found */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/sum_base7.h"

/* helper: sum of distinct prime divisors of n */
static uint64_t sum_distinct_prime_factors(uint64_t n) {
    uint64_t sum = 0;
    uint64_t tmp = n;
    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            sum += p;
            while (tmp % p == 0)
                tmp /= p;
        }
    }
    if (tmp > 1)        /* remaining prime factor */
        sum += tmp;
    return sum;
}

/* dynamic array of seq_t values */
static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* allocate or grow the seq array */
static void ensure_seq_cap(size_t needed) {
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* next integer to examine (starts from 2) */
static uint64_t next_candidate = 2;

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;     /* undefined for n=0, return 0 */

    /* generate terms until we have n of them */
    while (seq_len < (size_t)n) {
        uint64_t cand = next_candidate;

        uint64_t sb = sum_base7(cand);
        if (sb != 0) {        /* avoid division by zero */
            uint64_t sp = sum_distinct_prime_factors(cand);
            if (sp % sb == 0) {
                ensure_seq_cap(seq_len + 1);
                seq[seq_len++] = (seq_t)cand;
            }
        }

        ++next_candidate;
    }
    return seq[n - 1];
}
