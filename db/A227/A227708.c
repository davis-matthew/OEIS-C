/*
 * A227708  The Wiener index of the dendrimer D_n defined pictorially in Fig.
 *          1 of the Heydari et al. reference.
 * https://oeis.org/A227708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18 + 2^n*(66+30*n) + 36*n*4^n
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 11, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow2_exp(idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;
    while (exp > 0) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t n_seq   = (seq_t)n;
    seq_t pow2_n  = pow2_exp(n);
    seq_t pow4_n  = pow2_exp(n * 2); /* 4^n = 2^(2n) */
    seq_t term1   = pow2_n * ((seq_t)66 + (seq_t)30 * n_seq);
    seq_t term2   = (seq_t)36 * n_seq * pow4_n;
    return (seq_t)18 + term1 + term2;
}
