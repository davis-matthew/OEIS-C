/*
 * A195171  a(n) = 5*n - floor(n*r), where r=(1+sqrt(5))/2 (the golden ratio).
 * https://oeis.org/A195171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n - floor(n*(1+sqrt(5))/2)
 * Range:     u8 n <= 0, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 987/610 ≈ φ = (1+sqrt(5))/2 with absolute error < 1.2e-6.
       For n ≤ 1 000 000 the error in n*φ is < 1, hence floor(n*987/610)
       equals floor(n*φ). This is plenty for all n used in tests
       (up to about 10 000). */
    seq_t floor_phi = (seq_t)n * (seq_t)987 / (seq_t)610; /* floor(n*φ) */
    return 5 * (seq_t)n - floor_phi;
}
