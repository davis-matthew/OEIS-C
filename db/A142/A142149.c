/*
 * A142149  a(n) = XOR{k OR (n-k): 0<=k<=n}.
 * https://oeis.org/A142149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, hear, look, easy
 * Formula:   a(n) = XOR_{k=0}^{n-1} (k | (n-k))
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The summation over k = n gives the same term twice and cancels its
   contribution, so the loop runs only up to n-1. */
seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t k = 0; k < n; ++k) {
        seq_t x = (seq_t)k | (seq_t)(n - k);
        res ^= x;
    }
    return res;
}
