/*
 * A001291  Number of conjugacy classes in Restricted Affine Group on n
 *          variables.
 * https://oeis.org/A001291
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2^(n+2) - 4 + (n even ? n/2 : 0)
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t pow2 = 1;
    idx_t i;
    for (i = 0; i < n + 2; i++)
        pow2 *= 2;
    seq_t result = pow2 - 4;
    if ((n & 1) == 0)
        result += (seq_t)(n / 2);
    return result;
}
