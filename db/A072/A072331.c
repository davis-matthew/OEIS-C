/*
 * A072331  a(n) = 2^(n-1) * Sum_{k=0..n} ((n+k)!/(n-k)!)/k!.
 * https://oeis.org/A072331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n-1) * Sum_{k=0..n} ((n+k)!/(n-k)!)/k!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* the sequence is defined for n ≥ 1 */
    if (n == 0)
        return 0;

    seq_t sum = 1;      /* term for k = 0 */
    seq_t t   = 1;      /* current value T_k */

    for (idx_t k = 0; k < n; ++k) {
        /* T_{k+1} = T_k * (n-k) * (n+k+1) / (k+1) */
        seq_t num = t;
        num *= (seq_t)(n - k);
        num *= (seq_t)(n + k + 1);
        t = num / (seq_t)(k + 1);
        sum += t;
    }

    /* compute 2^(n-1) */
    seq_t pow2 = 1;
    for (idx_t i = 1; i <= n - 1; ++i)
        pow2 *= 2;

    return sum * pow2;
}
