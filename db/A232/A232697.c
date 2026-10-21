/*
 * A232697  Number of partitions of 2n into parts such that the largest
 *          multiplicity equals n.
 * https://oeis.org/A232697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (p(n) - p(n-1)), where p(k) is the partition
 *            function. p(k) is computed using Euler's pentagonal number
 *            theorem
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 151, u64 n <= 465,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for partition numbers. */
static seq_t *P = NULL;
static size_t P_capacity = 0;
static idx_t  P_size = 0;

/* Ensure that P[0..up_to] are defined. */
static void ensure_partitions(idx_t up_to)
{
    if (P_size > up_to)
        return;

    size_t needed = (size_t)up_to + 1;
    if (needed > P_capacity) {
        size_t new_cap = P_capacity == 0 ? 4 : P_capacity;
        while (new_cap < needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(P, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out‑of‑memory – kill the program */
        P = tmp;
        P_capacity = new_cap;
    }

    if (P_size == 0) {              /* initialise p(0) */
        P[0] = (seq_t)1;
        P_size = 1;
    }

    for (size_t i = P_size; i < needed; ++i) {
        seq_t sum_pos = (seq_t)0;
        seq_t sum_neg = (seq_t)0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            if (k & 1) sum_pos += P[i - pent1];
            else       sum_neg += P[i - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 <= i) {
                if (k & 1) sum_pos += P[i - pent2];
                else       sum_neg += P[i - pent2];
            }
        }
        P[i] = sum_pos - sum_neg;
    }
    P_size = needed;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 1 by definition */
        return (seq_t)1;

    ensure_partitions(n);
    /* a(n) = 1 + p(n) - p(n-1) */
    seq_t result = P[n] - P[n - 1] + (seq_t)1;
    return result;
}
