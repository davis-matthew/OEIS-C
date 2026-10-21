/*
 * A214856  Number of triangular numbers in interval ](n-1)^2, n^2] for n>0,
 *          a(0)=1.
 * https://oeis.org/A214856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ k | (k*(k+1))/2 ∈ ](n-1)^2 , n^2] }
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 5933, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Define a simple integer square‑root that works for any seq_t type. */
static inline seq_t isqrt_seq(seq_t x)
{
    seq_t low  = (seq_t)0;
    seq_t high = x + (seq_t)1;      /* high is exclusive          */
    while (low < high)
    {
        seq_t mid   = (low + high) / (seq_t)2;
        seq_t mid2  = mid * mid;
        if (mid2 <= x)
            low = mid + (seq_t)1;
        else
            high = mid;
    }
    return low - (seq_t)1;          /* floor of sqrt(x)           */
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)                          /* special case a(0)=1   */
        return (seq_t)1;

    /* m1 = n^2,  m2 = (n-1)^2 */
    seq_t m1 = (seq_t)n * (seq_t)n;
    idx_t n_minus = n - (idx_t)1;
    seq_t m2 = (seq_t)n_minus * (seq_t)n_minus;

    /* f(m) = floor((sqrt(1+8*m)-1)/2) */
    seq_t f1 = (isqrt_seq((seq_t)1 + (seq_t)8 * m1) - (seq_t)1) / (seq_t)2;
    seq_t f2 = (isqrt_seq((seq_t)1 + (seq_t)8 * m2) - (seq_t)1) / (seq_t)2;

    return f1 - f2;                             /* count of triangular numbers */
}
