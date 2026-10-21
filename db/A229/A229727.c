/*
 * A229727  Numerator of Sum_{k=1..2n+1} 2^k/k.
 * https://oeis.org/A229727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Sum_{k=1..2n+1} 2^k/k
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* limit = 2*n + 1 */
    idx_t limit = n;
    limit += limit;
    limit += 1;

    seq_t num = 0;        /* current numerator */
    seq_t den = 1;        /* current denominator */
    seq_t pow2 = 2;       /* 2^1 for k = 1 */

    for (idx_t step = 1; step <= limit; ++step)
    {
        seq_t term_den = (seq_t)step;            /* k */

        seq_t part1 = num * term_den;            /* num * k */
        seq_t part2 = den * pow2;                /* den * 2^k */

        seq_t new_num = part1 + part2;           /* new numerator before reduction */
        seq_t new_den = den * term_den;          /* new denominator before reduction */

        seq_t g = gcd(new_num, new_den);        /* common divisor */
        new_num /= g;
        new_den /= g;

        num = new_num;
        den = new_den;

        pow2 <<= 1;                              /* next power of two */
    }

    return num;
}
