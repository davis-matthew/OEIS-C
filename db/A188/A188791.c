/*
 * A188791  Number of 8-step self-avoiding walks on an n X n X n X n 4-cube
 *          summed over all starting positions.
 * https://oeis.org/A188791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 871256*n^4 - 5064008*n^3 + 10076736*n^2 - 8024256*n +
 *            2111616 for n>6
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small n handled by explicit values */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)24576;
    if (n == 3) return (seq_t)3203232;
    if (n == 4) return (seq_t)30437760;
    if (n == 5) return (seq_t)125478336;
    if (n == 6) return (seq_t)352052352;

    /* For n > 6 evaluate the 4‑degree polynomial.
       Computation done in 128‑bit to avoid overflow of intermediate
       products; the final result is converted to seq_t. */
    __int128 nn = ( __int128 ) n;
    __int128 res = 0;
    res += (__int128)871256   * nn * nn * nn * nn;
    res -= (__int128)5064008  * nn * nn * nn;
    res += (__int128)10076736 * nn * nn;
    res -= (__int128)8024256  * nn;
    res += 2111616;
    return (seq_t)res;
}
