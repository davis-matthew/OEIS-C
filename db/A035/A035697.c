/*
 * A035697  Number of partitions of n into parts 8k+5 and 8k+6 with at least
 *          one part of each type.
 * https://oeis.org/A035697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 8k+5 and 8k+6 with
 *            at least one part of each type
 * Range:     u8 n <= 88, u16 n <= 202, u32 n <= 557, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* static state */
static seq_t *P   = NULL;
static seq_t *P5  = NULL;
static seq_t *P6  = NULL;
static idx_t max_alloc = 0;

/* build all partitions up to size 'newsize' */
static void compute_up_to(idx_t newsize)
{
    seq_t *newP  = alloc_seq_array(newsize + 1);
    seq_t *newP5 = alloc_seq_array(newsize + 1);
    seq_t *newP6 = alloc_seq_array(newsize + 1);

    /* base case : one way to make 0 */
    newP[0]   = 1;
    newP5[0]  = 1;
    newP6[0]  = 1;

    /* add 5‑mod‑8 parts */
    for (idx_t k = 0; ; ++k) {
        idx_t v = 8 * k + 5;
        if (v > newsize) break;
        for (idx_t i = v; i <= newsize; ++i) {
            newP[i]  += newP[i - v];
            newP5[i] += newP5[i - v];
        }
    }

    /* add 6‑mod‑8 parts */
    for (idx_t k = 0; ; ++k) {
        idx_t v = 8 * k + 6;
        if (v > newsize) break;
        for (idx_t i = v; i <= newsize; ++i) {
            newP[i]  += newP[i - v];
            newP6[i] += newP6[i - v];
        }
    }

    /* free old arrays */
    if (P)  free_seq_array(P);
    if (P5) free_seq_array(P5);
    if (P6) free_seq_array(P6);

    /* store the new arrays */
    P = newP;
    P5 = newP5;
    P6 = newP6;
    max_alloc = newsize;
}

/* function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* the sequence is defined for n≥1 */
    if (n > max_alloc) compute_up_to(n);
    seq_t result = P[n];
    result -= P5[n];
    result -= P6[n];
    return result;
}
