/*
 * A221619  Number of n X 4 arrays with each row a permutation of 1..4 having
 *          at least as many downsteps as the preceding row.
 * https://oeis.org/A221619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*a(n-1) - 166*a(n-2) + 264*a(n-3) - 121*a(n-4) with
 *            initial values a(1)=24, a(2)=410, a(3)=6120, a(4)=85035. a(1) is
 *            the first term in the OEIS offset
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, used only for local variables */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 1) return (seq_t)24;
    if (n == 2) return (seq_t)410;
    if (n == 3) return (seq_t)6120;
    if (n == 4) return (seq_t)85035;

    /* Keep only the last four values */
    seq_t a1 = (seq_t)24;    /* a(n-4) */
    seq_t a2 = (seq_t)410;   /* a(n-3) */
    seq_t a3 = (seq_t)6120;  /* a(n-2) */
    seq_t a4 = (seq_t)85035; /* a(n-1) */
    seq_t val = a4;          /* placeholder for a(n) */

    for (idx_t i = 5; i <= n; ++i) {
        /* a(n) = 24*a(n-1) - 166*a(n-2) + 264*a(n-3) - 121*a(n-4) */
        val = ((seq_t)24 * a4) - ((seq_t)166 * a3) + ((seq_t)264 * a2) - ((seq_t)121 * a1);

        /* shift the window for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = val;
    }

    return val;
}
