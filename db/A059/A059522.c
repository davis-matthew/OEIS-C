/*
 * A059522  a(1) = 1, then a(n) = n*(n^(n-1)-1)*(n-2)!/(n-1).
 * https://oeis.org/A059522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (n^(n-1) - 1) * (n-2)! / (n-1) (a(1)=1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the special case n = 1 */
    if (n == 1u) return (seq_t)1;

    /* (n-2)!  */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n - 2; ++i)
        fact *= (seq_t)i;

    /* n^(n-1)  */
    seq_t power = (seq_t)1;
    seq_t base  = (seq_t)n;
    idx_t exp   = n - 1;
    for (idx_t iter = 0; iter < exp; ++iter)
        power *= base;

    /* (n^(n-1) - 1) / (n-1)  –  exact division */
    seq_t t = power - (seq_t)1;
    t /= (seq_t)(n - 1);

    /* Final result: t * n * fact  */
    seq_t res = t;
    res *= (seq_t)n;
    res *= fact;
    return res;
}
