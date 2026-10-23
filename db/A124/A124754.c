/*
 * A124754  Alternating sum of compositions in standard order.
 * https://oeis.org/A124754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabf
 * Formula:   a(n) = alternating sum of the n`th composition in standard
 *            order. Recurrence used: a(0) = 0 If n is a power of two, say n =
 *            2^m, then a(n) = m + 1. Otherwise let n = 2^m + 2^e2 + k (0 ≤ k
 *            < 2^e2 < 2^m) then a(n) = (m - e2) - a(2^e2 + k). The recursion
 *            strictly decreases the largest power of two p...
 * Range:     i8 n <= 9051, i16 n <= 9051, i32 n <= 9051, i64 n <= 9051,
 *            i128 n <= 9051, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t calc_a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* check for power of two: n & (n-1) == 0 */
    if ((n & (n - 1)) == 0) {
        /* compute floor(log2 n) */
        unsigned m = 63 - __builtin_clzll((unsigned long long)n);
        return (seq_t)(m + 1);
    }

    /* n is not a power of two */
    unsigned m = 63 - __builtin_clzll((unsigned long long)n);   /* highest bit */
    idx_t t = n - ((idx_t)1 << m);                              /* remainder after subtracting 2^m */

    /* t > 0 because n is not a power of two */
    unsigned e2 = 63 - __builtin_clzll((unsigned long long)t); /* highest bit of t */
    idx_t k = t - ((idx_t)1 << e2);

    idx_t sub_n = ((idx_t)1 << e2) + k;
    seq_t sub_val = calc_a(sub_n);
    seq_t result = (seq_t)(m - e2) - sub_val;
    return result;
}

seq_t a(idx_t n)
{
    return calc_a(n);
}
