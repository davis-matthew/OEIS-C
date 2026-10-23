/*
 * A129230  a(n) = floor(n*r) + floor((n-2)*r) + floor((n-4)*r) + ... +
 *          floor(k*r), where r = golden ratio = (1 + sqrt(5))/2 and k = n mod
 *          2.
 * https://oeis.org/A129230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*r) + floor((n-2)*r) + ... + floor(k*r), where r =
 *            golden ratio = (1 + sqrt(5))/2 and k = n mod 2
 * Range:     u8 n <= 3, u16 n <= 54, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

static seq_t floor_phi(idx_t m)
{
    seq_t k = (seq_t)m;          /* convert index to seq_t */
    seq_t sq = k * k;            /* k^2 */
    sq *= (seq_t)5;              /* 5 * k^2 */
    seq_t l = floor_sqrt(sq);    /* floor(k * sqrt(5)) = floor(sqrt(5*k^2)) */
    return (k + l) / (seq_t)2;   /* floor((k + floor(k*sqrt5)) / 2) */
}


seq_t a(idx_t n)
{
    seq_t result = (seq_t)0;
    idx_t parity = n & (idx_t)1;        /* n mod 2 */

    for (idx_t i = n; ; i -= (idx_t)2)  /* iterate n, n-2, ..., parity */
    {
        result += floor_phi(i);
        if (i == parity)
            break;
    }
    return result;
}
