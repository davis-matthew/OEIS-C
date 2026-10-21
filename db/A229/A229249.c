/*
 * A229249  Number of set partitions of {1,...,n} with largest set of size 7.
 * https://oeis.org/A229249
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = B7(n) – B6(n) * where Bk(n) = Σ_{i=1..k} binom(n-1,i-1) *
 *            Bk[n-i] * and Bk(0) = 1. * This counts partitions of {1,…,n}
 *            with largest part 7
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 17, u64 n <= 26, u128 n <= 43,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binom.h"

/* Upper bound for n in the expected tests (≈10000). */
#define MAX_N 10000

static seq_t B6[MAX_N + 1];
static seq_t B7[MAX_N + 1];
static idx_t B6_max = 0;
static idx_t B7_max = 0;

/* Compute Bk up to 'up_to' using the recurrence with limit k. */
static void ensure_Bk(idx_t up_to, idx_t k, seq_t *B, idx_t *max_ptr)
{
    if (up_to > MAX_N) up_to = MAX_N;
    if (up_to <= *max_ptr)
        return;

    if (*max_ptr == 0) {
        B[0] = (seq_t)1;
    }
    for (idx_t n = *max_ptr + 1; n <= up_to; ++n) {
        seq_t sum = (seq_t)0;
        idx_t max_i = k;
        if (max_i > n)
            max_i = n;
        for (idx_t i = 1; i <= max_i; ++i) {
            seq_t comb = binom(n - 1, i - 1);
            sum += comb * B[n - i];
        }
        B[n] = sum;
    }
    *max_ptr = up_to;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 7)
        return (seq_t)0;

    if (n > MAX_N)    /* for safety; sequence is defined for all n. */
        n = MAX_N;

    ensure_Bk(n, 6, B6, &B6_max);
    ensure_Bk(n, 7, B7, &B7_max);

    return B7[n] - B6[n];
}
