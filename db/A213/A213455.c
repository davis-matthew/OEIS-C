/*
 * A213455  90*A002451(n).
 * https://oeis.org/A213455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (15 - 384*4^n + 729*9^n) / 4 (equivalent to the
 *            recurrence a(n) = 14*a(n-1) - 49*a(n-2) + 36*a(n-3) for n > 2,
 *            with a(0)=90, a(1)=1260, a(2)=13230)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, mul_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/mul_uint.h"

seq_t a(idx_t n)
{
    /* Known low‑order terms */
    if (n == 0) return (seq_t)90;
    if (n == 1) return (seq_t)1260;
    if (n == 2) return (seq_t)13230;

    /* Compute 4^n and 9^n using the provided big_pow routine */
    seq_t exp = (seq_t)n;                  /* exponent as seq_t */
    seq_t pow4 = big_pow((seq_t)4, exp);
    seq_t pow9 = big_pow((seq_t)9, exp);

    /* 729*9^n and 384*4^n, using safer multiplication */
    seq_t term1 = mul_uint(pow9, 729);      /* 729 * 9^n   */
    seq_t term2 = mul_uint(pow4, 384);      /* 384 * 4^n   */

    /* Compute the numerator: 729*9^n - 384*4^n + 15 */
    seq_t numerator = (term1 - term2) + 15; /* guaranteed non‑negative */

    /* Denominator is 4; division is exact */
    return numerator / 4;
}
