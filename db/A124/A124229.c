/*
 * A124229  Numerator of g(n) defined by g(1)=1, g(2n)=1/g(n)+1,
 *          g(2n+1)=g(2n).
 * https://oeis.org/A124229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = A000045(ceiling(log(n+1)/log(2)) + 1)
 * Range:     u8 n <= 4095, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, a(n) is the Fibonacci number F(k) where
   k = 1 + ceil(log2(n+1)).  Fibonacci indexing starts with
   F(1)=1, F(2)=1, F(3)=2, ...  */

/* External integer types defined by the build system. */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* Compute k = 1 + ceil(log2(n+1)) */
    idx_t m = n + (idx_t)1;               /* m = n+1, guaranteed >0 for n>=1 */

    /* Find t such that 2^t <= m < 2^(t+1).  t is floor(log2(m)). */
    int t = 0;
    while (t < 63 && ((m >> (t + 1)) != 0ULL))
        ++t;

    /* Check if m is an exact power of two. */
    idx_t pow = ((idx_t)1) << t;          /* 2^t */
    int ceil_log = (m == pow) ? t : t + 1;

    int k = ceil_log + 1;                 /* desired Fibonacci index */

    /* Compute F(k) iteratively (F(1)=F(2)=1) */
    if (k <= 2)
        return (seq_t)1;                  /* F(1)=F(2)=1 */

    seq_t f_prev = (seq_t)1;              /* F(1) */
    seq_t f_curr = (seq_t)1;              /* F(2) */
    for (int i = 3; i <= k; ++i)
    {
        seq_t f_next = f_prev + f_curr;   /* F(i) = F(i-1)+F(i-2) */
        f_prev = f_curr;
        f_curr = f_next;
    }
    return f_curr;                        /* F(k) */
}
