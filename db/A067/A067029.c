/*
 * A067029  Exponent of least prime factor in prime factorization of n,
 *          a(1)=0.
 * https://oeis.org/A067029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = exponent of the least prime factor in the factorisation
 *            of n, a(1) = 0. The function uses the helper from lpf_uint64.h
 *            to obtain the least prime factor of a 64‑bit integer and then
 *            counts how many times that factor divides n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   lpf_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/lpf_uint64.h"

seq_t a(idx_t n)
{
    /* a(1) is defined to be 0 */
    if (n <= 1)
        return 0;

    uint64_t t = (uint64_t)n;
    uint64_t p = lpf_uint64(t);          /* least prime factor of t */

    seq_t exp = 0;
    while (t % p == 0) {
        t /= p;
        ++exp;
    }

    return exp;
}
