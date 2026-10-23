/*
 * A174863  Little omega analog to Liouville's function L(n).
 * https://oeis.org/A174863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{i=1..n} (-1)^omega(i) (omega = number of distinct
 *            prime factors)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t *cum = NULL;   /* cumulative sums a(0)=0, a(1).. */
static idx_t cur_max = 0;   /* largest n for which cum is valid */

/* Recompute a(1..n) and store in cum */
static void recompute(idx_t n) {
    /* Allocate new array for cumulative sums */
    seq_t *new_cum = (seq_t *)malloc(sizeof(seq_t)*(n+1));
    if (!new_cum) return; /* allocation failed – let caller handle it */
    new_cum[0] = (seq_t)0;

    /* Compute omega(i) for 1..n by marking distinct prime factors */
    unsigned char *omega = (unsigned char *)calloc((size_t)(n+1), sizeof(unsigned char));
    if (!omega) {
        free(new_cum);
        return;
    }

    for (idx_t p = 2; p <= n; ++p) {
        if (omega[p] == 0) { /* p is prime */
            for (idx_t j = p; j <= n; j += p) {
                omega[j] += 1;
            }
        }
    }

    /* Build cumulative sums from omega values */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t contrib = (omega[i] & 1) ? (seq_t)(-1) : (seq_t)(1);
        new_cum[i] = new_cum[i-1] + contrib;
    }

    free(omega);

    /* Replace old array */
    if (cum) free(cum);
    cum = new_cum;
    cur_max = n;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;        /* not an official index, but safe */
    if (n > cur_max) recompute(n);
    return cum[n];
}
