/*
 * A048985  Working in base 2, replace n with the concatenation of its prime
 *          divisors in increasing order (write answer in base 10).
 * https://oeis.org/A048985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, base, look
 * Formula:   a(n) = concatenate the binary representations of the prime
 *            factors of n (including multiplicities) in increasing order and
 *            interpret the result as a binary number
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the number of bits needed to represent x in binary. */
static unsigned bitlen_u64(uint64_t x)
{
    unsigned len = 0;
    while (x) { x >>= 1; ++len; }
    return len;
}

/* Shift v left by s bits (s <= 64).  Works for fixed‑width seq_t as
   well as for arbitrary‑precision types that overload <<. */
static seq_t shift_left(seq_t v, unsigned s)
{
    while (s--) v <<= 1;
    return v;
}

seq_t a(idx_t n)
{
    /* Special case: 1 has no prime divisors, but is defined to be 1. */
    if (n == 1) return (seq_t)1;

    seq_t result = 0;
    uint64_t tmp = (uint64_t)n;

    /* Handle factor 2 separately to keep the loop over odd numbers. */
    while ((tmp & 1) == 0) {
        result = shift_left(result, 2);         /* 2 has 2 bits in binary ("10") */
        result |= (seq_t)2;
        tmp >>= 1;
    }

    /* Factor the remaining odd part. */
    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        while (tmp % p == 0) {
            unsigned bits = bitlen_u64(p);
            result = shift_left(result, bits);
            result |= (seq_t)p;
            tmp /= p;
        }
    }

    /* If a prime factor greater than sqrt(n) remains. */
    if (tmp > 1) {
        unsigned bits = bitlen_u64(tmp);
        result = shift_left(result, bits);
        result |= (seq_t)tmp;
    }

    return result;
}
