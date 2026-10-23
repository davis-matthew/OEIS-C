/*
 * A190910  a(n) = Sum{0<=k<=n} binomial(n+k,n-k) * k! / (floor(k/2)!)^2.
 * https://oeis.org/A190910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A190910
   Formula: a(n) = Sum_{k=0..n} binomial(n+k, n-k) * k! / (floor(k/2)!)^2
   (computed by a multiplicative recurrence on the summand) */

seq_t a(idx_t n)
{
    /* t holds T(n,k) = binomial(n+k, n-k) * k! / (floor(k/2)!)^2
       sum holds the cumulative sum a(n)                      */
    seq_t sum = (seq_t)1;  /* k = 0 term is always 1                     */
    seq_t t   = (seq_t)1;  /* T(n,0)                                     */

    for (idx_t k = (idx_t)1; k <= n; ++k) {
        /* factor1 = (n+k)*(n-k+1) / ((2k)*(2k-1))                */
        seq_t a = (seq_t)(n + k);
        seq_t b = (seq_t)(n - k + 1);

        seq_t tmp = t * a;
        tmp       = tmp * b;

        seq_t denom = (seq_t)(2 * k);
        denom = denom * (seq_t)(2 * k - 1);

        tmp = tmp / denom;  /* exact division */

        /* ratio2 = k             if k odd
                     4/k         if k even */
        if ((k & (idx_t)1) == (idx_t)0) {
            seq_t num = tmp * (seq_t)4;
            seq_t den = (seq_t)k;
            t = num / den;   /* exact division */
        } else {
            t = tmp * (seq_t)k;
        }

        sum = sum + t;
    }

    return sum;
}
