/*
 * A178924  Sum_{k>0} (n mod k) * 2^(n-k).
 * https://oeis.org/A178924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + sum_{k=1..n} (n mod k) * 2^(n-k)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t pow = 1;               /* current 2^(n-k) for current k */
    for (idx_t k = n; k > 0; --k)
    {
        seq_t mod = (seq_t)(n % k);  /* n mod k fits in idx_t */
        sum += mod * pow;
        pow *= 2;
    }
    sum += (seq_t)n;              /* contribution from k>n */
    return sum;
}
