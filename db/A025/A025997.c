/*
 * A025997  Expansion of 1/((1-2*x)*(1-5*x)*(1-8*x)*(1-9*x)).
 * https://oeis.org/A025997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n)=24*a(n-1)-201*a(n-2)+674*a(n-3)-720*a(n-4),
   a(0)=1, a(1)=24, a(2)=375, a(3)=4850, suitable for all n>=0. */

#include <stdlib.h>

static seq_t *seq = NULL;
static idx_t seq_cap = 0;
static idx_t seq_len = 0;

seq_t a(idx_t n)
{
    /* Initialise cache with first four terms. */
    if (seq_len == 0) {
        seq_cap = 4;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)24;
        seq[2] = (seq_t)375;
        seq[3] = (seq_t)4850;
        seq_len = 4;
    }

    /* Return cached value if already computed. */
    if (n < seq_len)
        return seq[n];

    /* Compute subsequent terms up to n. */
    for (idx_t i = seq_len; i <= n; ++i) {
        if (i >= seq_cap) {
            idx_t new_cap = seq_cap ? seq_cap << 1 : 4;
            seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            seq_cap = new_cap;
        }
        seq_t val = (seq_t)24 * seq[i-1]
                  - (seq_t)201 * seq[i-2]
                  + (seq_t)674 * seq[i-3]
                  - (seq_t)720 * seq[i-4];
        seq[i] = val;
        seq_len = i + 1;
    }

    return seq[n];
}
