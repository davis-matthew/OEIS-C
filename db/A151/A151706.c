/*
 * A151706  a(0)=1, a(1)=0; a(2^i+j) = 2*a(j) + 2*a(j+1) for 0 <= j < 2^i.
 * https://oeis.org/A151706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 1016, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=0;
   a(2^i + j) = 2*a(j) + 2*a(j+1)   for all i>=1 and 0 <= j < 2^i */
#include <stdlib.h>

static seq_t *seq = NULL;   /* cache of computed terms               */
static idx_t seq_len = 0;   /* length of allocated cache array      */

/* Build or extend the cache so that indices 0..n are defined. */
static void compute_seq_up_to(idx_t n)
{
    if (seq_len <= n) {
        /* Allocate space for 0..n inclusive */
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_len * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure should never happen for the test sizes. */
            abort();
        }
        seq = tmp;
        seq_len = new_len;

        /* Initialise the two starting values */
        seq[0] = (seq_t)1;
        if (seq_len > 1) seq[1] = (seq_t)0;

        /* Compute the remaining values using the recurrence. */
        for (idx_t i = 1; (1ULL << i) < seq_len; ++i) {
            idx_t block_start = 1ULL << i;       /* 2^i          */
            idx_t block_size  = 1ULL << i;       /* also 2^i      */
            for (idx_t j = 0; j < block_size; ++j) {
                idx_t idx = block_start + j;
                if (idx >= seq_len) break;
                seq[idx] = (seq_t)2 * seq[j] + (seq_t)2 * seq[j + 1];
            }
        }
    }
}

seq_t a(idx_t n)
{
    compute_seq_up_to(n);
    return seq[n];
}
