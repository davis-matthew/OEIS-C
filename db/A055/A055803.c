/*
 * A055803  a(n) = T(n,n-3), array T as in A055801.
 * https://oeis.org/A055803
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-39 +55*n -15*n^2 +2*n^3 +(-1)^n*(135 -39*n +3*n^2))/96
 *            for n>3
 * Range:     u8 n <= 25, u16 n <= 149, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the special first terms explicitly (offset is 3). */
    if (n <= 5)
        return (seq_t)1;

    /* Compute the formula using 128‑bit arithmetic to avoid overflow. */
    __int128 nn   = ( __int128 ) n;
    __int128 term = -39
                    + 55 * nn
                    - 15 * nn * nn
                    +  2 * nn * nn * nn;

    /* Add or subtract the alternating part according to n's parity. */
    if ((n & 1) == 0)          /* even n */
        term +=   135 - 39 * nn + 3 * nn * nn;
    else                        /* odd n */
        term -=   135 - 39 * nn + 3 * nn * nn;

    /* Integer division by 96 (the result is always divisible). */
    term /= 96;

    return (seq_t)term;
}
