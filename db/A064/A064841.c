/*
 * A064841  Working in base 2, replace n with the concatenation of its prime
 *          divisors in increasing order.
 * https://oeis.org/A064841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = decimal value of the binary concatenation of the prime
 *            divisors of n (each divisor repeated according to its exponent)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* Helper: number of bits needed to represent a positive 64‑bit integer. */
static inline unsigned bitlen_uint64(uint64_t v)
{
    unsigned len = 0;
    while (v) {
        v >>= 1;
        ++len;
    }
    return len;
}

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition. */
    if (n <= 1)
        return (seq_t)1;

    uint64_t x = (uint64_t)n;
    seq_t res = 0;

    /* Handle factor 2 separately for speed. */
    while ((x & 1ULL) == 0) {
        /* Binary representation of 2 is "10". */
        res = res * (seq_t)10 + (seq_t)1;
        res = res * (seq_t)10 + (seq_t)0;
        x >>= 1;
    }

    /* Trial division for odd primes. */
    for (uint64_t p = 3; p * p <= x; p += 2) {
        while (x % p == 0) {
            /* Append the binary digits of p. */
            unsigned len = bitlen_uint64(p);
            uint64_t mask = ((uint64_t)1 << (len - 1));
            for (unsigned i = 0; i < len; ++i) {
                seq_t bit = (mask & p) ? (seq_t)1 : (seq_t)0;
                res = res * (seq_t)10 + bit;
                mask >>= 1;
            }
            x /= p;
        }
    }

    /* Remaining prime factor, if any. */
    if (x > 1) {
        uint64_t p = x;
        unsigned len = bitlen_uint64(p);
        uint64_t mask = ((uint64_t)1 << (len - 1));
        for (unsigned i = 0; i < len; ++i) {
            seq_t bit = (mask & p) ? (seq_t)1 : (seq_t)0;
            res = res * (seq_t)10 + bit;
            mask >>= 1;
        }
    }

    return res;
}
