/*
 * A086985  a(n) = A000407(n) - A086984(n).
 * https://oeis.org/A086985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n-1)!/(n-1)! - n! - (2^(n-1)-1)*n*n!
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset: a(1) is the first listed term */
    if (n < 1) return (seq_t)0;

    /* ---- factorial n! ---- */
    seq_t fac = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= n; ++i)
        fac *= (seq_t)i;

    /* ---- product (2n‑1)!/(n‑1)! = Π_{k=n}^{2n‑1} k ---- */
    seq_t prod = (seq_t)1;
    for (idx_t k = n; k <= 2 * n - (idx_t)1; ++k)
        prod *= (seq_t)k;

    /* ---- power 2^(n‑1) ---- */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= n - (idx_t)1; ++i)
        pow2 *= (seq_t)2;

    /* ---- compute the subtractive term (2^(n-1)-1)*n*n! ---- */
    seq_t temp = pow2 - (seq_t)1;
    temp *= (seq_t)n;
    temp *= fac;

    /* ---- final result ---- */
    return prod - fac - temp;
}
