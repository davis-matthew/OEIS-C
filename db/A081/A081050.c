/*
 * A081050  a(n) = (n! - Stirling1(n+1, 2))*(-1)^n where Stirling1 is A048994.
 * https://oeis.org/A081050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * ((-1)^n + H_n), where H_n = sum_{k=1..n} 1/k and H_0
 *            = 0
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute factorial n! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    /* compute sum_{k=1..n} n!/k  (integer division is exact) */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
        sum += fact / (seq_t)k;

    /* add or subtract factorial depending on parity of n  */
    if (n & 1ULL)   /* n odd: subtract factorial */
        return sum - fact;
    else            /* n even: add factorial */
        return sum + fact;
}
