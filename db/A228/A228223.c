/*
 * A228223  Number of second differences of arrays of length 8 of numbers in
 *          0..n.
 * https://oeis.org/A228223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 84*n^6 - 402*n^4 + 1656*n^3 - 1860*n^2 + 776*n + 1 (OEIS
 *            A228223)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 19, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use signed 128‑bit arithmetic to avoid overflow in intermediate
       calculations while the final result fits in seq_t for the valid range. */
    __int128 nn = (__int128)n;
    __int128 n2 = nn * nn;          /* n² */
    __int128 n3 = n2 * nn;          /* n³ */
    __int128 n4 = n2 * n2;          /* n⁴ */
    __int128 n6 = n3 * n3;          /* n⁶ */

    __int128 val = 84   * n6
                 - 402  * n4
                 + 1656 * n3
                 - 1860 * n2
                 + 776  * nn
                 + 1;

    return (seq_t)val;
}
