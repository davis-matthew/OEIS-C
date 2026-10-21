/*
 * A212682  Number of (w,x,y,z) with all terms in {1,...,n} and |x-y|>=|y-z|.
 * https://oeis.org/A212682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A212682: Number of tuples (w,x,y,z) with 1 ≤ w,x,y,z ≤ n
 * satisfying |x - y| ≥ |y - z|.  
 * Modeled directly by combinatorial counting:
 *     a(n) = n * Σ_{y=1..n} S(y)
 * where S(y) = 2(m+1)^2 - 1 + d*(4m+3+d)/2,
 *     m = min(y-1, n-y),  d = |(y-1) - (n-y)|.
 */

seq_t a(idx_t n)
{
    seq_t f_sum = (seq_t)0;
    for (idx_t y = (idx_t)1; y <= n; ++y)
    {
        idx_t a = y - (idx_t)1;
        idx_t b = n - y;
        idx_t m = (a < b) ? a : b;
        idx_t d = (a > b) ? (a - b) : (b - a);

        seq_t mseq = (seq_t)m;
        seq_t dseq = (seq_t)d;

        seq_t term = ((seq_t)2) * (mseq + (seq_t)1) * (mseq + (seq_t)1) - (seq_t)1;
        seq_t add  = (dseq * (((seq_t)4) * mseq + (seq_t)3 + dseq)) / (seq_t)2;
        term += add;

        f_sum += term;
    }

    return (seq_t)n * f_sum;
}
