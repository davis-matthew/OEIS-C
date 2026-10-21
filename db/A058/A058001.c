/*
 * A058001  Number of 3 X 3 matrices with entries mod n, up to row and column
 *          permutation.
 * https://oeis.org/A058001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^9 + 6*n^6 + 9*n^5 + 8*n^3 + 12*n^2) / 36
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 17, u64 n <= 205, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: 3!^2 = 36.  The numerator is always divisible by 36 for integer n. */

seq_t a(idx_t n)
{
    /* Compute all powers using 128‑bit arithmetic to avoid overflow. */
    unsigned __int128 tn = (unsigned __int128)n;        /* n         */
    unsigned __int128 t2 = tn * tn;                    /* n^2       */
    unsigned __int128 t3 = t2 * tn;                    /* n^3       */
    unsigned __int128 t5 = t3 * t2;                    /* n^5       */
    unsigned __int128 t6 = t5 * tn;                    /* n^6       */
    unsigned __int128 t9 = t6 * t3;                    /* n^9       */

    /* Numerator of the formula. */
    unsigned __int128 num = t9
                         + (unsigned __int128)6 * t6
                         + (unsigned __int128)9 * t5
                         + (unsigned __int128)8 * t3
                         + (unsigned __int128)12 * t2;

    /* Division by 36 (3!^2).  The result is integer. */
    unsigned __int128 res = num / 36;

    return (seq_t)res;
}
