/*
 * A078157  a(n) = A078152(2^n).
 * https://oeis.org/A078157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(0, floor(sqrt(2^(n+2))) - n - 2)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   big_pow, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"      /* seq_t big_pow(seq_t base, seq_t exp) */
#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t x) */

seq_t a(idx_t n)
{
    /* 2^(n+2) as a big integer */
    seq_t exp  = (seq_t)(n + 2);
    seq_t base = (seq_t)2;
    seq_t power = big_pow(base, exp);

    /* floor(√(2^(n+2))) */
    seq_t sqrtv = floor_sqrt(power);

    /* n + 2 */
    seq_t sub = (seq_t)n + (seq_t)2;

    /* a(n) = max(0, sqrtv - sub) */
    return (sqrtv > sub) ? (sqrtv - sub) : (seq_t)0;
}
