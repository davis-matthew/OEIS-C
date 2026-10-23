/*
 * A129821  Triangle read by rows: T(n, k) = (n-k)^n - n*k*(n-k) + k^n, with
 *          T(0, 0) = 1.
 * https://oeis.org/A129821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(t,k) where * t = ⌊(√(8n+1)-1)/2⌋ and k = n - t(t+1)/2 *
 *            T(n,k) = (n-k)^n - n*k*(n-k) + k^n, with T(0,0)=1 * The sequence
 *            is the triangle read by rows
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 54, u64 n <= 135, u128 n <= 377,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the row index t such that t(t+1)/2 ≤ n < (t+1)(t+2)/2 */
    idx_t low  = 0;
    idx_t high = n + 1;          /* exclusive upper bound */

    while (low + 1 < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t t = low;
    idx_t k = n - t * (t + 1) / 2;

    /* Special case T(0,0) = 1 */
    if (t == 0)
        return (seq_t)1;

    seq_t seq_t_t = (seq_t)t;
    seq_t seq_t_k = (seq_t)k;

    seq_t base1 = seq_t_t - seq_t_k;  /* n-k */
    seq_t base2 = seq_t_k;           /* k */

    seq_t pow1 = big_pow(base1, seq_t_t);   /* (n-k)^n */
    seq_t pow2 = big_pow(base2, seq_t_t);   /* k^n   */

    seq_t mul = seq_t_t;
    mul = mul * seq_t_k;
    mul = mul * (seq_t_t - seq_t_k);        /* n*k*(n-k) */

    return pow1 + pow2 - mul;
}
