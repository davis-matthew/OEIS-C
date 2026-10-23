/*
 * A154055  Number of planar triangular n X n X n nonnegative integer grids
 *          with mirror symmetry about one altitude with every similarly
 *          oriented 3 X 3 X 3 subtriangle summing to 6.
 * https://oeis.org/A154055
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   For n >= 12 the values repeat with period 6: 35, 32, 35, 29, 38,
 *            29 For 3 <= n <= 11 the values are: 30, 67, 78, 94, 85, 83, 54,
 *            55, 32 a(n) is undefined for n < 3 (returns 0)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    if (n <= 11)
    {
        static const seq_t pre[9] = {
            30, 67, 78, 94, 85, 83, 54, 55, 32
        };
        return pre[n - 3];
    }

    static const seq_t pat[6] = {35, 32, 35, 29, 38, 29};
    return pat[n % 6];
}
