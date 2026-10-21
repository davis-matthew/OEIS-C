/*
 * A213562  Principal diagonal of the convolution array A213561.
 * https://oeis.org/A213562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 109, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) - a(n-6)
 * Initial values (offset 1): a(1)=1, a(2)=18, a(3)=109, a(4)=407, a(5)=1152, a(6)=2723
 */
#include <stddef.h>   /* for idx_t */

seq_t a(idx_t n)
{
    /* a(1)..a(6) are required for all n>=1. For n==0 we return 0. */
    if (n == 0) return 0;

    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)18;
    if (n == 3) return (seq_t)109;
    if (n == 4) return (seq_t)407;
    if (n == 5) return (seq_t)1152;
    if (n == 6) return (seq_t)2723;

    /* Keep the last six terms: a(n-6) ... a(n-1) */
    seq_t a1 = (seq_t)1;     /* a(n-6) when n=7 */
    seq_t a2 = (seq_t)18;    /* a(n-5) */
    seq_t a3 = (seq_t)109;   /* a(n-4) */
    seq_t a4 = (seq_t)407;   /* a(n-3) */
    seq_t a5 = (seq_t)1152;  /* a(n-2) */
    seq_t a6 = (seq_t)2723;  /* a(n-1) */

    for (idx_t i = 7; i <= n; ++i) {
        seq_t an = 6 * a6
                 - 15 * a5
                 + 20 * a4
                 - 15 * a3
                 + 6 * a2
                 - a1;
        /* shift window forward */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = an;
    }
    return a6;
}
