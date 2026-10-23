/*
 * A101463  Expansion of g.f. (x^3+x^2+2*x+1)/(x^4+5*x^2+1).
 * https://oeis.org/A101463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 27, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0)=1, a(1)=2, a(2)=-4, a(3)=-9,
 *         a(n) = -5*a(n-2) - a(n-4)   for n >= 4
 */
#include <stdlib.h>

static seq_t *seq = NULL;      /* sequence storage */
static idx_t  len  = 0;        /* number of elements currently computed */
static idx_t  cap  = 0;        /* current allocated capacity */

seq_t a(idx_t n)
{
    /* If already computed, return immediately */
    if (n < len)
        return seq[n];

    /* Initialise the first four values if not done yet */
    if (len == 0) {
        cap = 4;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!seq) return 0;          /* allocation failure: undefined behaviour */
        seq[0] =  (seq_t) 1;
        seq[1] =  (seq_t) 2;
        seq[2] = (seq_t)-4;
        seq[3] = (seq_t)-9;
        len = 4;
    }

    /* Grow the array if necessary and compute missing terms */
    while (len <= n) {
        if (len >= cap) {
            cap = (cap == 0 ? 4 : cap * 2);
            seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
            if (!seq) return 0; /* allocation failure */
        }
        seq_t val = -5 * seq[len - 2] - seq[len - 4];
        seq[len] = val;
        ++len;
    }

    return seq[n];
}
