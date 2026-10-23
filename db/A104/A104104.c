/*
 * A104104  a(1) = 1, if A(k) = sequence of first 2^(k-1) terms and if B(k) is
 *          A(k) with 0's and 1's exchanged, then A(k+1) = A(k)A(k) if a(k) =
 *          0, A(k+1) = A(k)B(k) if a(k) = 1.
 * https://oeis.org/A104104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1.
   For k ≥ 1, let A(k) be the first 2^(k-1) terms of the sequence.
   Let B(k) be A(k) with 0’s and 1’s exchanged.
   Then A(k+1) = A(k)A(k) if a(k) = 0,
               A(k+1) = A(k)B(k) if a(k) = 1. */

#include <stdlib.h>
#include <stdint.h>

static uint8_t *seq = NULL;   /* current sequence stored as 0/1 bytes   */
static idx_t   seq_len = 0;  /* length of seq ( = 2^(k-1) )           */
static idx_t   k = 1;        /* current k such that seq_len = 2^(k-1) */

seq_t a(idx_t n)
{
    /* ----- initialise on the first call -------------------------- */
    if (seq == NULL) {
        seq = (uint8_t *)malloc(1);
        seq[0] = 1;           /* a(1) */
        seq_len = 1;
        k = 1;
    }

    /* ----- extend sequence until it covers index n ----------------- */
    while (seq_len < n) {
        uint8_t a_k = seq[k - 1];        /* a(k) value */

        size_t new_len = (size_t)seq_len * 2;
        uint8_t *new_seq = (uint8_t *)malloc(new_len);

        /* copy the first half */
        for (idx_t i = 0; i < seq_len; ++i) {
            new_seq[i] = seq[i];
        }

        /* fill the second half according to a(k) */
        if (a_k == 0) {
            for (idx_t i = 0; i < seq_len; ++i) {
                new_seq[seq_len + i] = seq[i];
            }
        } else {
            for (idx_t i = 0; i < seq_len; ++i) {
                new_seq[seq_len + i] = seq[i] ^ 1;   /* complement */
            }
        }

        free(seq);
        seq = new_seq;
        seq_len = new_len;
        ++k;
    }

    /* ----- return the n‑th term ---------------------------------- */
    return (seq_t)seq[n - 1];
}
