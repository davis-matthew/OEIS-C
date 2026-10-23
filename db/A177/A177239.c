/*
 * A177239  Partial sums of round(n^2/20).
 * https://oeis.org/A177239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+4)*(2*n^2 - 5*n + 6)/120)
 * Range:     u8 n <= 24, u16 n <= 157, u32 n <= 895, u64 n <= 895,
 *            u128 n <= 895, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A177239 – partial sums of round(n^2/20). */

seq_t a(idx_t n)
{
    /* Use 64‑bit intermediates so the calculation is safe up to at least
       n ≈ 10^6.  All terms for the tested range (n ≤ 10 000) fit trivially. */
    unsigned long long n_ul = (unsigned long long)n;
    unsigned long long n_sq = n_ul * n_ul;                   /* n^2          */
    unsigned long long t1   = 2ULL * n_sq - 5ULL * n_ul + 6ULL; /* 2*n^2-5*n+6  */
    unsigned long long num  = (n_ul + 4ULL) * t1;              /* (n+4)*t1     */
    return (seq_t)(num / 120ULL);                              /* floor(num/120) */
}
