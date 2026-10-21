/*
 * A066728  a(n) is the number of integers of the form (n+k+n*k)/(n-k) for k =
 *          1,2,...,n-1.
 * https://oeis.org/A066728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil( d( n*(n+2) ) / 2 ) - 1, where d(m) is the number of
 *            divisors of m
 * Range:     u8 n <= 5849, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* safety for invalid indices */

    /* Compute m = n * (n + 2) as 64‑bit unsigned integer. */
    uint64_t nn = (uint64_t)n;
    uint64_t m  = nn * (nn + 2ULL);

    /* Factorization of m to compute the divisor count d. */
    uint64_t temp = m;
    uint64_t d = 1;                /* d starts at 1 (empty product) */

    uint64_t p = 2;
    while (p * p <= temp) {
        uint64_t cnt = 0;
        while (temp % p == 0) {
            temp /= p;
            ++cnt;
        }
        if (cnt > 0) d *= (cnt + 1);
        if (p == 2) p = 3;        /* try 3 next */
        else        p += 2;        /* then only odd candidates */
    }
    if (temp > 1) d *= 2;          /* remaining prime factor */

    /* a(n) = ceil(d/2) - 1  ==>  (d + 1)/2 - 1  */
    seq_t ans = (seq_t)((d + 1ULL) / 2ULL) - 1;
    return ans;
}
