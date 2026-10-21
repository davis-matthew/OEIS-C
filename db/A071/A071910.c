/*
 * A071910  a(n) = t(n)*t(n+1)*t(n+2), where t() are the triangular numbers.
 * https://oeis.org/A071910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = t(n) * t(n+1) * t(n+2) where t(n) = n*(n+1)/2
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 55, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the OEIS sequence A071910. */

seq_t a(idx_t n)
{
    /* Compute each triangular number as seq_t to avoid overflow of idx_t. */
    seq_t t1 = (seq_t)n * (n + 1) / 2;          /* t(n)     */
    seq_t t2 = (seq_t)(n + 1) * (n + 2) / 2;    /* t(n+1)   */
    seq_t t3 = (seq_t)(n + 2) * (n + 3) / 2;    /* t(n+2)   */

    /* Compute the product. For 64‑bit builds this is safe for all n
       where the final result fits in uint64_t; for larger n the value
       does not fit and the caller is not required to check. */
    return t1 * t2 * t3;
}
