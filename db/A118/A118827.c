/*
 * A118827  2-adic continued fraction of zero, where a(n) = 1 if n is odd,
 *          otherwise -2*A006519(n/2).
 * https://oeis.org/A118827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cofr, sign, mult
 * Formula:   a(n) = 1 if n is odd; otherwise -2*A006519(n/2), where
 *            A006519(k) is the highest power of 2 dividing k (i.e. 2^v2(k))
 * Range:     i8 n <= 255, i16 n <= 65535, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 115 terms through n = 9020 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the largest power of 2 dividing n (n > 0).  Returns 2^v2(n). */
static seq_t power_of_two_divisor(idx_t n)
{
    seq_t power = 1;
    while ((n & 1ULL) == 0) {
        n >>= 1;
        power <<= 1;
    }
    return power;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                  /* offset starts at 1; behaviour for 0 is irrelevant */
    if ((n & 1ULL) == 1ULL)
        return 1;                  /* odd indices */
    idx_t m = n >> 1;              /* n/2 */
    seq_t p = power_of_two_divisor(m);
    return -((seq_t)2 * p);        /* even indices */
}
