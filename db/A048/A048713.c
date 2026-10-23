/*
 * A048713  3rd row of Family 1 "90 x 150 array": generations 0 .. n of Rule
 *          90 starting from seed pattern 21.
 * https://oeis.org/A048713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 21; for n ≥ 1, a(n) = a(n-1) XOR (a(n-1) << 2)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 21;
    for (idx_t i = 0; i < n; ++i)
        val ^= val << 2;
    return val;
}
