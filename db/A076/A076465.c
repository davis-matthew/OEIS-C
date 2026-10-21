/*
 * A076465  Sum of squares of numbers that can be written as t*n + u*(n+1) for
 *          nonnegative integers t,u in exactly n ways.
 * https://oeis.org/A076465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)*(6*n^6+12*n^5-5*n^4-16*n^3+0*n^2+5*n+1)/6
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 255, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.
       For n == 0 we simply return 0; the caller never uses this value. */
    if (n == 0)
        return 0;

    /* Use a signed 128‑bit integer for intermediate steps to avoid overflow
       while keeping the code simple.  This type is available on GCC/Clang
       and compiles fine in C++17 as well. */
    __int128 nn = (__int128)n;
    __int128 poly = 6;           /* coefficient of n^6 */

    poly = poly * nn + 12;       /* +12*n^5   */
    poly = poly * nn - 5;        /* -5*n^4    */
    poly = poly * nn - 16;       /* -16*n^3   */
    poly = poly * nn;            /* +0*n^2    */
    poly = poly * nn + 5;        /* +5*n      */
    poly = poly * nn + 1;        /* +1        */

    __int128 prod = poly * nn * (nn + 1);
    prod /= 6;                   /* final division */

    return (seq_t)prod;
}
