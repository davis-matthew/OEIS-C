/*
 * A154699  Terms in A014217 pairwise swapped.
 * https://oeis.org/A154699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2n) = A014217(2n+1), a(2n+1) = A014217(2n) Recurrence for
 *            A014217 (b): b(n) = b(n-1) + 2*b(n-2) - b(n-3) - b(n-4) base:
 *            b(0)=1, b(1)=1, b(2)=2, b(3)=4 a(n) is obtained by swapping
 *            pairs of b
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache for A014217 values */
    static seq_t *b = NULL;
    static idx_t b_len = 0;               /* number of computed entries (0..b_len-1) */

    /* maximum index of b needed for requested n */
    idx_t needed = (n & 1U) ? n - 1 : n + 1;

    /* initialise cache */
    if (b == NULL) {
        b_len = 4;
        b = (seq_t *)calloc(b_len, sizeof(seq_t));
        if (!b) exit(1);
        b[0] = (seq_t)1;   /* b(0) */
        b[1] = (seq_t)1;   /* b(1) */
        b[2] = (seq_t)2;   /* b(2) */
        b[3] = (seq_t)4;   /* b(3) */
    }

    /* grow cache if necessary */
    if (b_len <= needed) {
        /* allocate enough space */
        size_t new_cap = (size_t)needed + 1;
        seq_t *tmp = (seq_t *)realloc(b, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        b = tmp;

        /* compute new entries using the recurrence */
        for (idx_t i = b_len; i <= needed; ++i) {
            seq_t t = b[i - 1] + b[i - 2] + b[i - 2];  /* b[i-1] + 2*b[i-2] */
            t -= b[i - 3];
            t -= b[i - 4];
            b[i] = t;
        }
        b_len = needed + 1;
    }

    /* return the swapped pair value */
    return (n & 1U) ? b[n - 1] : b[n + 1];
}
