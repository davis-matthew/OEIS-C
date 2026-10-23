/*
 * A024916  a(n) = Sum_{k=1..n} k*floor(n/k); also Sum_{k=1..n} sigma(k) where
 *          sigma(n) = sum of divisors of n (A000203).
 * https://oeis.org/A024916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, changed
 * Formula:   a(n) = Sum_{k=1..n} k * floor(n/k)
 * Range:     u8 n <= 17, u16 n <= 281, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t q = (seq_t)(n / k);
        res += ((seq_t)k) * q;
    }
    return res;
}
