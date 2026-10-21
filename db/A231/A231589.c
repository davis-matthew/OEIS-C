/*
 * A231589  a(n) = sum_{k=1..(n-1)/2} (k^2 mod n).
 * https://oeis.org/A231589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..(n-1)/2} (k^2 mod n)
 * Range:     u8 n <= 32, u16 n <= 512, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t limit = (n - 1) / 2;
    const seq_t mod = (seq_t)n;

    for (idx_t k = 1; k <= limit; ++k) {
        seq_t k_sq = (seq_t)k * (seq_t)k;
        sum += k_sq % mod;
    }
    return sum;
}
