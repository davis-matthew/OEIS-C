/*
 * A088482  a(n) = Sum_{k=1..4} floor(p(n,k) / p(n-1,k)) where p(n,k) = n! /
 *          (n-floor(n/2^k))!.
 * https://oeis.org/A088482
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = Σ_{k=1..4} ⌊ p(n,k) / p(n−1,k) ⌋, where p(n,k) = n! /
 *            (n−⌊n/2^k⌋)!. For each k, the ratio simplifies to n if n ≡ 0
 *            (mod 2^k), ⌊ n / (n−⌊n/2^k⌋) ⌋ otherwise
 * Range:     u8 n <= 63, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Sequence is undefined for n < 2; return 0 for safety */
    if (n < 2)
        return (seq_t)0;

    seq_t sum = 0;

    for (int k = 1; k <= 4; ++k)
    {
        /* 2^k as an unsigned integer */
        uint64_t pow2 = 1ULL << k;

        if (n % pow2 == 0)
        {
            /* Exact division case */
            sum += (seq_t)n;
        }
        else
        {
            /* Compute floor(n / (n - floor(n/2^k))) */
            idx_t f = n / pow2;               /* floor(n/2^k) */
            idx_t m = n - f;                  /* n - floor(n/2^k) */
            sum += (seq_t)(n / m);            /* integer division gives the floor */
        }
    }

    return sum;
}
