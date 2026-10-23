/*
 * A140497  a(n) = 2p - number of digits of p^p, where p = n-th prime.
 * https://oeis.org/A140497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = 2 * p – floor(p * log10(p)) – 1, where p = n-th prime
 * Range:     i8 n <= 50, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;                /* undefined for n = 0 */

    uint64_t p = (uint64_t)get_prime(n);

    /* Compute number of decimal digits of p^p:  floor(p*log10(p)) + 1 */
    long double ld = (long double)p;
    long double v  = ld * log10l(ld);     /* p * log10(p) */
    const long double eps = 1e-12L;       /* safety margin for rounding */
    unsigned long long digits = (unsigned long long)(v + eps) + 1ULL;

    /* Safe intermediate calculation in a wider signed integer. */
    int64_t tmp = (int64_t)2 * (int64_t)p;           /* 2 * p  */
    int64_t res = tmp - (int64_t)digits;           /* a(n)  */

    return (seq_t)res;
}
