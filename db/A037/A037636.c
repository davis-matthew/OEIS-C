/*
 * A037636  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,3.
 * https://oeis.org/A037636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{k=0}^{n-1} d_k * 8^{n-1-k} where d_k cycles through
 *            2, 1, 3 in order
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t pattern[3] = {2, 1, 3};
    seq_t value = 0;
    int idx = 0;
    for (idx_t i = 0; i < n; ++i) {
        value = value * 8 + pattern[idx];
        idx = (idx + 1) % 3;
    }
    return value;
}
