/*
 * A047210  Largest square modulo n.
 * https://oeis.org/A047210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = max_{0 <= k < n} k^2 mod n
 * Range:     u8 n <= 161, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* safety for indices below offset */

    seq_t N = (seq_t)n;
    seq_t max = 0;

    for (idx_t k = 0; k < n; ++k) {
        seq_t sq = ((seq_t)k * (seq_t)k) % N;
        if (sq > max) max = sq;
    }
    return max;
}
