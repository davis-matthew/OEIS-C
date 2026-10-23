/*
 * A153065  Prime numbers such that the sum of any 5 consecutive terms is a
 *          prime.
 * https://oeis.org/A153065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A153065: Prime numbers such that the sum of any 5 consecutive
   terms is a prime.  The function a(n) returns the n‑th such prime
   (n ≥ 1).  The sequence is built iteratively: after the first five
   terms, each new term is the smallest prime greater than the
   predecessor for which the sum of the last five terms (including
   the new one) is prime. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n) */

static seq_t *seq          = NULL;   /* storage for the sequence */
static idx_t seq_len       = 0;      /* number of terms stored          */
static size_t seq_cap      = 0;      /* allocated capacity (by count)   */

static void ensure_seq(idx_t n)
/* Make sure the global array contains the first n terms of the sequence. */
{
    while (seq_len < n) {
        /* Allocate initial storage if needed. */
        if (!seq) {
            seq_cap = 16;
            seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
            if (!seq) exit(EXIT_FAILURE);
        }
        /* Grow the array *before* writing beyond existing capacity. */
        if (seq_len == seq_cap) {
            seq_cap *= 2;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            if (!seq) exit(EXIT_FAILURE);
        }

        /* First five terms are known explicitly. */
        if (seq_len < 5) {
            static const seq_t init[5] = {(seq_t)3, (seq_t)5, (seq_t)11,
                                          (seq_t)19, (seq_t)23};
            seq[seq_len] = init[seq_len];
        } else {
            /* Sum of the last four terms. */
            seq_t sum4 = seq[seq_len-1] + seq[seq_len-2]
                       + seq[seq_len-3] + seq[seq_len-4];

            /* Find the next prime that keeps the 5‑term sum prime. */
            seq_t cand = seq[seq_len-1] + (seq_t)2;   /* start with next odd */
            for (;;) {
                if (is_prime_big(cand)) {
                    seq_t total = sum4 + cand;
                    if (is_prime_big(total)) {
                        seq[seq_len] = cand;
                        break;
                    }
                }
                cand += (seq_t)2;
            }
        }
        seq_len++;
    }
}

seq_t a(idx_t n)   /* n ≥ 1 */
{
    ensure_seq(n);
    return seq[n-1];
}
