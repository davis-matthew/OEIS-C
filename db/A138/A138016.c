/*
 * A138016  Row sums of triangle A138015.
 * https://oeis.org/A138016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A003422(ceil(k/2))
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 26, u64 n <= 42, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum by iterating factorials and partial sums up to floor(n/2). */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not used, but safe */

    /* m = floor(n/2) is needed for the recursive formulas */
    idx_t m = n / 2;

    /* fact   = i!          (initially 0! = 1)
       S      = sum_{t=0..i} t!  (initially S_0 = 1)
       sumS   = sum_{i=0}^{m-1} S_i   (the cumulative sum of S_i) */
    seq_t fact = (seq_t)1;   /* 0! = 1 */
    seq_t S    = fact;       /* S_0 = 1 */
    seq_t sumS = (seq_t)0;   /* sum of S_i for i < m */

    /* If m > 0, accumulate S_0 and then iterate up to i = m-1 */
    if (m > 0) {
        sumS += S;                      /* add S_0 */

        for (idx_t i = 1; i < m; ++i) { /* i ranges 1 .. m-1 */
            fact *= (seq_t)i;           /* i!  (i ≥ 1) */
            S += fact;                  /* update S_i */
            sumS += S;                  /* add S_i to the cumulative sum */
        }
    }

    if ((n & 1) == 0) {                  /* even n: a(2*m) = 2 * sumS */
        return sumS * (seq_t)2;
    } else {                              /* odd n: a(2*m+1) = 2*sumS + S_m */
        if (m > 0) {                      /* need S_m = S_{m-1} + m!   */
            fact *= (seq_t)m;             /* m!  (fact currently (m-1)!) */
            S += fact;                    /* now S = S_m */
        }
        return sumS * (seq_t)2 + S;
    }
}
