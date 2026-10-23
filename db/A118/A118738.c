/*
 * A118738  Number of ones in binary expansion of 5^n.
 * https://oeis.org/A118738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = Hamming weight of 5ⁿ (number of 1‑bits in the binary
 *            expansion of 5ⁿ)
 * Range:     u8 n <= 209, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* special case: 5⁰ = 1 has a single 1‑bit */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Very small usage of external memory; all arithmetic done with native types. */
    /* Each limb holds 32 bits; the magnitude grows roughly as n·log₂5 bits. */
    /* For n ≤ 10000 we need < 8000 limbs; allocate a bit more than necessary. */
    /* 8000 limbs × 4 bytes = 32 KiB, comfortably small. */
    const size_t MAX_LIMBS = 8000;
    uint32_t *limb = (uint32_t *)malloc(MAX_LIMBS * sizeof(uint32_t));
    if (limb == NULL) {
        /* In a contest environment a failing malloc is considered fatal. */
        abort();
    }
    size_t len = 1;
    limb[0] = 1;          /* start with 5⁰ = 1 */

    for (idx_t step = 0; step < n; ++step) {
        uint64_t carry = 0;
        for (size_t i = 0; i < len; ++i) {
            uint64_t prod = (uint64_t)limb[i] * 5 + carry;
            limb[i] = (uint32_t)prod;   /* lower 32 bits */
            carry   = prod >> 32;       /* higher 32 bits */
        }
        if (carry) {
            /* Append carry as a new limb.  For 5*max_limb+carry we need at most
               a single extra limb because 5*(2³²-1)+4 < 2³³. */
            limb[len++] = (uint32_t)carry;
        }
    }

    /* Count 1-bits in the resulting big integer. */
    seq_t count = 0;
    for (size_t i = 0; i < len; ++i) {
#if defined(__GNUC__) || defined(__clang__)
        count += __builtin_popcount(limb[i]);
#else
        /* Portable popcount fallback */
        uint32_t x = limb[i];
        x = x - ((x >> 1) & 0x55555555);
        x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
        x = (x + (x >> 4)) & 0x0F0F0F0F;
        count += (x * 0x01010101) >> 24;
#endif
    }

    free(limb);
    return count;
}
