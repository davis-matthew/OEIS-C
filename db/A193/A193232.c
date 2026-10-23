/*
 * A193232  Bitwise XOR of first n triangular numbers.
 * https://oeis.org/A193232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = XOR_{k=1}^{n} k*(k+1)/2 (with a(0) = 0)
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* a(0) is defined to be 0 */
        return 0;
    }

    seq_t xor_val = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* Compute the k-th triangular number in seq_t to avoid overflow. */
        seq_t tri = (seq_t)k * ((seq_t)k + 1) / 2;
        xor_val ^= tri;
    }
    return xor_val;
}
