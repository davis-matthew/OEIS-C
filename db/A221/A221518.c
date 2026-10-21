/*
 * A221518  Number of 0..n arrays of length 7 with each element differing from
 *          at least one neighbor by 2 or more, starting with 0.
 * https://oeis.org/A221518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 + n^5 - 29*n^4 + 104*n^3 - 173*n^2 + 136*n - 40 for n
 *            > 3 (the first three terms are a(1)=0, a(2)=8, a(3)=248)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 40, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)248;

    /* Use the empirical polynomial for all n ≥ 4.
       All intermediate products fit in the arbitrary‑precision
       type seq_t, so no overflow occurs.               */
    seq_t sn = (seq_t)n;

    /* Compute powers of n.  Using temporary variables
       avoids repeated multiplications.                  */
    seq_t n2 = sn * sn;            /* n^2      */
    seq_t n3 = n2 * sn;            /* n^3      */
    seq_t n4 = n3 * sn;            /* n^4      */
    seq_t n5 = n4 * sn;            /* n^5      */
    seq_t n6 = n5 * sn;            /* n^6      */

    /* Separate the positive and negative parts
       to avoid underflow when seq_t is unsigned.   */
    seq_t pos = n6 + n5 + (seq_t)104 * n3 + (seq_t)136 * sn;
    seq_t sub = (seq_t)29 * n4 + (seq_t)173 * n2 + (seq_t)40;

    return pos - sub;
}
