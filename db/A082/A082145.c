/*
 * A082145  A subdiagonal of number array A082137.
 * https://oeis.org/A082145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(n-1) + (0^n)/2) * C(2*n+3, n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          For n >= 1: a(n) = 2^(n-1) * C(2*n+3, n)       */
/*          For n = 0 : a(0) = 1                            */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;                /* 2^(-1)+1/2 = 1, multiplied by C(3,0)=1 */
    }

    /* Compute the binomial coefficient C(2*n+3, n) multiplicatively */
    idx_t k, N = 2 * n + 3;
    seq_t binom = 1;

    for (k = 1; k <= n; ++k) {
        seq_t numerator   = (seq_t)(N - (k - 1));   /* (2*n+3) - (k-1) */
        seq_t denominator = (seq_t)k;
        binom = binom * numerator / denominator;    /* exact division */
    }

    /* Multiply by 2^(n-1) – simply a left shift operation */
    binom <<= (n - 1);

    return binom;
}
