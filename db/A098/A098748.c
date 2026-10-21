/*
 * A098748  a(n) = floor((n^4-n^3-1)/(n^2-n-1)).
 * https://oeis.org/A098748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n^4 - n^3 - 1) / (n^2 - n - 1))
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use 128‑bit arithmetic for intermediate results to avoid overflow */
    __int128_t n128 = n;
    __int128_t num = n128 * n128 * n128 * n128 - n128 * n128 * n128 - 1;
    __int128_t den = n128 * n128 - n128 - 1;
    __int128_t res = num / den;   /* integer division truncates toward zero, which equals the floor for these values */
    return (seq_t)res;
}
