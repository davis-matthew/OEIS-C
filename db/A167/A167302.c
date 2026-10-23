/*
 * A167302  Totally multiplicative sequence with a(p) = 10*(p-2) for prime p.
 * https://oeis.org/A167302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   multiplicative with a(p^e) = (10*(p-2))^e, a(2k)=0 for k≥1
 * Range:     u8 n <= 14, u16 n <= 224, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* a(1)=1 */
    if ((n & 1) == 0) return (seq_t)0;    /* even numbers give 0 */

    seq_t result = (seq_t)1;
    uint64_t temp   = (uint64_t)n;

    while (temp > 1) {
        /* find the least odd prime factor of temp */
        uint64_t p = 0;
        for (uint64_t d = 3; d * d <= temp; d += 2) {
            if (temp % d == 0) { p = d; break; }
        }
        if (p == 0) p = temp;            /* temp itself is prime */

        /* count exponent of p */
        int exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }

        /* multiply by (10*(p-2))^exp */
        seq_t factor = (seq_t)(10 * ((seq_t)p - (seq_t)2));
        for (int i = 0; i < exp; ++i) result *= factor;
    }

    return result;
}
