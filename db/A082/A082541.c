/*
 * A082541  a(n) = (7*3^n - 4*0^n)/3.
 * https://oeis.org/A082541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*3^n - 4*0^n)/3
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;               /* a(0) = 1 */

    seq_t value = 7;            /* a(1) = 7 */
    for (idx_t i = 1; i < n; ++i)
        value *= 3;             /* a(n) = 7 * 3^(n-1) for n ≥ 1 */

    return value;
}
