/*
 * A180479  Numbers m such that m/k is an integer. m = (x_1 x_2 ... x_r) where
 *          x_i are digits of m, k = x_1*r + x_2*(r-1) + ... + x_r*1.
 * https://oeis.org/A180479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1314, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   weighted_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A180479:
   Numbers m such that m is divisible by its weighted sum of digits,
   where the weight of the i‑th digit (from the most significant) is
   r-i+1, with r the number of digits.
   The function returns the n‑th such integer (1‑indexed). */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/weighted_sum.h"

static seq_t *seq = NULL;        /* array of found terms          */
static size_t seq_len = 0;       /* current number of terms       */
static size_t seq_cap = 0;       /* allocated capacity           */
static uint64_t next_candidate = 1;   /* next integer to test        */

/* Increase the storage capacity when needed. */
static void grow(void) {
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 4;
        seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* Return the n‑th term of sequence A180479. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined – use 0 as placeholder */

    size_t target = (size_t)n;
    while (seq_len < target) {
        seq_t m = (seq_t)next_candidate;
        seq_t k = weighted_sum(m);
        if (k != 0 && m % k == 0) {
            grow();
            seq[seq_len++] = m;
        }
        next_candidate++;
    }
    return seq[n - 1];
}
