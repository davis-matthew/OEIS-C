/*
 * A212974  Number of (w,x,y) with all terms in {0,...,n} and
 *          w>floor((x+y)/3).
 * https://oeis.org/A212974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - 3*a(n-4) + 3*a(n-5) -
 *            a(n-6). initial values: a(0)=0, a(1)=4, a(2)=15, a(3)=37,
 *            a(4)=75, a(5)=132
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /*  Sequence values for n = 0..5  */
    static const seq_t init[6] = { (seq_t)0, (seq_t)4, (seq_t)15,
                                  (seq_t)37, (seq_t)75, (seq_t)132 };

    if (n < 6)
        return init[n];

    /*  Current 6 consecutive values: a(k-6)..a(k-1)  */
    seq_t a0 = init[0]; /* a(k-6)  */
    seq_t a1 = init[1]; /* a(k-5)  */
    seq_t a2 = init[2]; /* a(k-4)  */
    seq_t a3 = init[3]; /* a(k-3)  */
    seq_t a4 = init[4]; /* a(k-2)  */
    seq_t a5 = init[5]; /* a(k-1)  */

    seq_t an = 0;

    for (idx_t k = 6; k <= n; ++k)
    {
        /* Compute a(k) using the given recurrence */
        seq_t left  = 3 * a5 + 2 * a3 + 3 * a1;
        seq_t right =   3 * a4 + 3 * a2 + a0;
        an = left - right;          /* subtraction is safe: left ≥ right */

        /* Shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }

    return an;
}
