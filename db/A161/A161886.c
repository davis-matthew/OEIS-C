/*
 * A161886  Number of nonzero elements in the n X n Redheffer matrix.
 * https://oeis.org/A161886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-1 + Σ_{i=1}^{n} ⌊n/i⌋ (number of non‑zero entries in
 *            the n×n Redheffer matrix)
 * Range:     u8 n <= 49, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For safety, treat n=0 as 0 (although OEIS offset starts at 1). */
    if (n == 0) return (seq_t)0;

    seq_t S = 0;          /* Σ floor(n/i) */
    idx_t i = 1;
    while (i <= n) {
        idx_t q    = n / i;          /* floor(n/i) */
        idx_t last = n / q;          /* largest j with floor(n/j)==q */
        S += (seq_t)(last - i + 1) * (seq_t)q;
        i = last + 1;
    }

    seq_t result = (seq_t)n - 1 + S;
    return result;
}
