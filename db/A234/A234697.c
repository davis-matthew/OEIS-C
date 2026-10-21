/*
 * A234697  Number of (n+1) X (1+1) 0..7 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 5
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=6*a(n-1)+90*a(n-2)-540*a(n-3)-2016*a(n-4)+12096*a(n-5) for
 *            n≥6. Initial terms: a(1)=408, a(2)=2628, a(3)=16848,
 *            a(4)=109224, a(5)=704160
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first five terms explicitly. */
    if (n == 1) return 408;
    if (n == 2) return 2628;
    if (n == 3) return 16848;
    if (n == 4) return 109224;
    if (n == 5) return 704160;

    /* Keep the last five sequence values in a sliding window. */
    seq_t v[5] = {
        (seq_t)408,      /* a(n-5) */
        (seq_t)2628,     /* a(n-4) */
        (seq_t)16848,    /* a(n-3) */
        (seq_t)109224,   /* a(n-2) */
        (seq_t)704160    /* a(n-1) */
    };

    for (idx_t i = 6; i <= n; ++i)
    {
        /* Compute the five-term linear recurrence.  All intermediate
           results fit in the same type without overflow.  We first
           accumulate the positive contributions and then subtract the
           negative ones; the positive sum is guaranteed to be
           at least as large as the negative sum for all n≥6, so no
           unsigned underflow occurs. */
        seq_t pos = (seq_t)6 * v[4]   /* a(n-1)    */
                  + (seq_t)90 * v[3]  /* a(n-2)    */
                  + (seq_t)12096 * v[0]; /* a(n-5) */
        seq_t neg = (seq_t)540 * v[2]   /* a(n-3)    */
                  + (seq_t)2016 * v[1]; /* a(n-4)    */
        seq_t next = pos - neg;       /* a(n)      */

        /* Slide the window: shift values left and insert the new term. */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = next;
    }

    return v[4]; /* a(n) after the loop terminates */
}
