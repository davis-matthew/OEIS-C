/*
 * A097967  a(n) = Sum_{k=1..n} (P(n,k) + C(n,k)).
 * https://oeis.org/A097967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (P(n,k) + C(n,k)) = Sum_{k=1..n} n!/(n-k)! +
 *            2^n - 1 Recurrence for the permutation sum S(n): S(0)=0, S(n) =
 *            n * S(n-1) + n. Then a(n) = S(n) + 2^n - 1. Compute each term in
 *            O(n) time
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t S      = 0;   /* sum of permutations */
    seq_t pow2   = 1;   /* 2^0 */
    seq_t result = 0;   /* a(n) when finished */

    for (idx_t i = 1; i <= n; ++i) {
        /* S = i * S + i */
        S = (seq_t)i * S + (seq_t)i;
        /* Update powers of two */
        pow2 = pow2 * 2;
        /* a(i) = S + 2^i - 1 */
        result = S + pow2 - 1;
    }
    return result;
}
