/*
 * A193823  Triangular array: the fusion of polynomial sequences P and Q given
 *          by p(n,x)=(2x+1)^n and q(n,x)=x^n+x^(n-1)+...+x+1.
 * https://oeis.org/A193823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2) and k = n -
 *            r*(r+1)/2, 0 ≤ k ≤ r. T(r,k) = ∑_{i=0..k} binomial(r-1, i) * 2^i
 *            for r ≥ 1, T(0,0) = 1
 * Range:     u8 n <= 31, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No external utilities required. */

seq_t a(idx_t n)
{
    /* Find the row index r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Number of terms preceding row r. */
    idx_t prev = r * (r + 1) / 2;
    idx_t k    = n - prev;          /* 0 ≤ k ≤ r */

    /* Special case: first row (r == 0). */
    if (r == 0)
        return (seq_t)1;

    /* Compute T(r,k) = Σ_{i=0..k} C(r-1,i) * 2^i. */
    seq_t sum   = 0;
    seq_t binom = 1;   /* C(r-1,0) */
    seq_t pow2  = 1;   /* 2^0 */

    for (idx_t i = 0; i <= k; ++i) {
        sum += binom * pow2;
        if (i == k)
            break;
        /* Update binomial coefficient: C(r-1,i+1) = C(r-1,i) * (r-1-i)/(i+1) */
        binom = binom * (seq_t)(r - 1 - i) / (seq_t)(i + 1);
        pow2 <<= 1;
    }
    return sum;
}
