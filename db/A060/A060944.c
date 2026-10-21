/*
 * A060944  a(n) = n!^2 * Sum_{k=1..n} Sum_{j=1..k} 1/j^2.
 * https://oeis.org/A060944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n!)^2 * sum_{k=0..n-1} (k+1)/(n-k)^2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1.  For n == 0 return 0, though the offset is 1. */
    if (n == 0)
        return (seq_t)0;

    /* Compute n! as a seq_t. */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* Sum the terms. */
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k < n; ++k)
    {
        idx_t d   = n - k;                      /* d = n-k  */
        seq_t tmp = fact / (seq_t)d;            /* fact / d  */
        seq_t sq  = tmp * tmp;                  /* (fact/d)^2 */
        sum += sq * (seq_t)(k + 1);             /* add term */
    }

    return sum;
}
