/*
 * A212512  Number of (w,x,y,z) with all terms in {1,...,n} and w<=2x and
 *          y<3z.
 * https://oeis.org/A212512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = f(n) * g(n) where f(n) = Σ_{x=1}^{n} min(2x, n) = m*(m+1)
 *            + (n-m)*n, with m = ⌊n/2⌋ g(n) = Σ_{z=1}^{n} min(n, 3z-1) =
 *            (p*(3p+1))/2 + (n-p)*n, with p = ⌊(n+1)/3⌋ These closed‑form
 *            expressions count the admissible pairs (w,x) and (y,z)
 *            independently, then multiply the counts
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 287, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n / 2;                /* floor(n/2)   */
    idx_t p = (n + 1) / 3;          /* floor((n+1)/3) */

    /* Convert to seq_t to avoid type mismatch in arithmetic */
    seq_t n_seq = (seq_t)n;
    seq_t m_seq = (seq_t)m;
    seq_t p_seq = (seq_t)p;

    /* f(n) = m*(m+1) + (n-m)*n */
    seq_t f = m_seq * (m_seq + 1) + ((seq_t)(n - m)) * n_seq;

    /* g(n) = (p*(3p+1))/2 + (n-p)*n */
    seq_t t = ((seq_t)(3 * p)) + 1;          /* 3p+1 */
    seq_t g = (p_seq * t) / (seq_t)2 + ((seq_t)(n - p)) * n_seq;

    return f * g;
}
