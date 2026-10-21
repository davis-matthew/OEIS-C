/*
 * A061717  Binary order of n^n.
 * https://oeis.org/A061717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ceil(log₂(nⁿ)) = bitlen(nⁿ) – (is_power_of_two(n) ? 1 :
 *            0)
 * Range:     u8 n <= 46, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special–case the first term of the sequence. */
    if (n <= 1)
        return (seq_t)0;

    /* Big integer represented as a dynamic array of 64‑bit limbs (little‑endian). */
    size_t cap = 4;
    size_t len = 1;
    uint64_t *limbs = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!limbs) return (seq_t)0;   /* out of memory – return 0 as a safe fallback */
    limbs[0] = 1ULL;

    uint64_t mul = (uint64_t)n;

    /* Compute nⁿ by multiplying n, n times. */
    for (idx_t i = 0; i < n; ++i)
    {
        unsigned __int128 carry = 0;
        for (size_t j = 0; j < len; ++j)
        {
            unsigned __int128 prod = (unsigned __int128)limbs[j] * mul + carry;
            limbs[j] = (uint64_t)prod;
            carry   = prod >> 64;
        }
        /* Append any remaining carried words. */
        while (carry)
        {
            if (len == cap)
            {
                cap <<= 1;
                limbs = (uint64_t *)realloc(limbs, cap * sizeof(uint64_t));
                if (!limbs) return (seq_t)0;
            }
            limbs[len++] = (uint64_t)carry;
            carry >>= 64;
        }
    }

    /* Compute the bit length of nⁿ. */
    size_t msb_index = len - 1;
    uint64_t msb = limbs[msb_index];
    unsigned int msb_bits = 64 - __builtin_clzll(msb);
    unsigned long long bitlen = (unsigned long long)msb_index * 64ULL + msb_bits;

    bool is_pow2 = (n & (n - 1)) == 0;
    unsigned long long result = bitlen - (is_pow2 ? 1ULL : 0ULL);

    free(limbs);
    return (seq_t)result;
}
