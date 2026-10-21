/*
 * A058344  Difference between the sum of the odd aliquot divisors of n and
 *          the sum of the even aliquot divisors of n.
 * https://oeis.org/A058344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = 2*σ(m) - σ(n) + (-1)^n * n, where m is the odd part of n
 *            (n = 2^k * m, m odd)
 * Range:     i8 n <= 95, i16 n <= 9802, i32 n <= 9802, i64 n <= 9802,
 *            i128 n <= 9802, big any n
 * Helpers:   sigma_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, offset starts at 1 */
    uint64_t u = (uint64_t)n;

    /* odd part of n */
    uint64_t m = u;
    while ((m & 1ULL) == 0) m >>= 1;

    seq_t sigma_m = sigma_uint(m);
    seq_t sigma_n = sigma_uint(u);

    seq_t diff = sigma_m + sigma_m; /* 2*σ(m) */
    diff -= sigma_n;

    if ((u & 1ULL) == 0)            /* n even */
        diff += (seq_t)u;
    else                            /* n odd */
        diff -= (seq_t)u;

    return diff;
}
