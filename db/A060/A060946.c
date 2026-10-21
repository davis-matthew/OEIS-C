/*
 * A060946  Trace of Vandermonde matrix of numbers 1,2,...,n, i.e., the matrix
 *          A with A[i,j] = i^(j-1), 1 <= i <= n, 1 <= j <= n.
 * https://oeis.org/A060946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^(k-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) as the sum of k^(k-1) for k = 1..n.
 * Uses binary exponentiation to keep the number of
 * multiplications to O(log k) per term. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t term = 1;
        seq_t base = k;
        idx_t exp = k - 1;
        while (exp) {
            if (exp & 1U) term *= base;
            base *= base;
            exp >>= 1;
        }
        sum += term;
    }
    return sum;
}
