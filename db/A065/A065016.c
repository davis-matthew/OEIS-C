/*
 * A065016  Working in base 2, replace n with the concatenation of its prime
 *          factors (without repetition).
 * https://oeis.org/A065016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the decimal integer obtained by concatenating (in
 *            ascending order) * the binary representations of the distinct
 *            prime factors of n. * For example, n=10 → factors 2,5 → binary
 *            "10" and "101" → concatenated * string "10101" → decimal 10101
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   pow10_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10_bits.h"

/* number of bits required to represent x (x>0) */
static unsigned bitlen_uint64(uint64_t x)
{
    unsigned len = 0;
    while (x) {
        ++len;
        x >>= 1;
    }
    return len;
}

/* convert an integer x to the decimal integer whose decimal digits are the
 * bits of x written in binary (most significant bit first). */
static seq_t bin_to_decimal_uint64(uint64_t x)
{
    unsigned bits = bitlen_uint64(x);
    seq_t out = 0;
    for (int i = (int)bits - 1; i >= 0; --i) {
        out = out * 10 + ((x >> i) & 1);
    }
    return out;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t temp = (uint64_t)n;
    seq_t result = 0;

    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            /* process prime p once (distinct factor) */
            unsigned bits = bitlen_uint64(p);
            seq_t bin = bin_to_decimal_uint64(p);
            seq_t pow10 = pow10_bits(bits);
            result = result * pow10 + bin;

            while (temp % p == 0) temp /= p;
        }
    }

    if (temp > 1) { /* remaining prime factor greater than sqrt(original n) */
        unsigned bits = bitlen_uint64(temp);
        seq_t bin = bin_to_decimal_uint64(temp);
        seq_t pow10 = pow10_bits(bits);
        result = result * pow10 + bin;
    }

    return result;
}
