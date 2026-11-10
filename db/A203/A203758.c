/*
 * A203758  Square root of v(2n+1)/(2*v(2n)), where v=A203755.
 * https://oeis.org/A203758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 * Product_{k=1..n-1} (k^4 + n^4)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n4 = n2 * n2;
    seq_t result = n4;

    for (idx_t k = 1; k < n; ++k) {
        seq_t K = (seq_t)k;
        seq_t k2 = K * K;
        result *= (k2 * k2 + n4);
    }

    return result;
}
