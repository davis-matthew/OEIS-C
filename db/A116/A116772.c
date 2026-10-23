/*
 * A116772  Number of permutations of length n which avoid the patterns 1234,
 *          1243, 1432.
 * https://oeis.org/A116772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 17*a(n-2) + 21*a(n-3) - 13*a(n-4) + 2*a(n-5) *
 *            for n ≥ 7 * Initial values: a(1)=1, a(2)=2, a(3)=6, a(4)=21,
 *            a(5)=76, a(6)=278
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t, if needed */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is the first term */
    if (n == 0)
        return (seq_t)0;        /* undefined for the sequence, but harmless */

    /* Return the known base terms outright */
    switch (n) {
        case 1: return (seq_t)1;
        case 2: return (seq_t)2;
        case 3: return (seq_t)6;
        case 4: return (seq_t)21;
        case 5: return (seq_t)76;
        case 6: return (seq_t)278;
        default: break;
    }

    /* Variables to hold the five previous terms; init to the six base terms */
    seq_t a1 = (seq_t)1;   /* a(n-5)  */
    seq_t a2 = (seq_t)2;   /* a(n-4)  */
    seq_t a3 = (seq_t)6;   /* a(n-3)  */
    seq_t a4 = (seq_t)21;  /* a(n-2)  */
    seq_t a5 = (seq_t)76;  /* a(n-1)  */
    seq_t a6 = (seq_t)278; /* a(n)    */

    /* Iterate from n = 7 up to the requested n */
    for (idx_t i = 7; i <= n; ++i) {
        seq_t a7 = (seq_t)7 * a6
                 - (seq_t)17 * a5
                 + (seq_t)21 * a4
                 - (seq_t)13 * a3
                 + (seq_t)2  * a2;

        /* Shift the window of last five terms */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = a7;
    }

    return a6;
}
