/*
 * A025706  Index of 5^n within the sequence of the numbers of the form
 *          4^i*5^j.
 * https://oeis.org/A025706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + n + Σ_{k=1..n} ⌊k * log_4(5)⌋
 * Range:     u8 n <= 20, u16 n <= 335, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  log_4(5) = ln(5)/ln(4) = log_2(5)/2                                            */
/*  The implementation builds 5^k iteratively, uses its bit length to obtain    */
/*  ⌊k * log_4(5)⌋ = ⌊ (bitlen(5^k)-1)/2 ⌋, and accumulates the sum.         */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* multiply an array of 32‑bit limbs by a small limb value (5).                */
static void mul_small(uint32_t *limbs, size_t *len, uint32_t mul)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < *len; ++i) {
        uint64_t cur = (uint64_t)limbs[i] * mul + carry;
        limbs[i] = (uint32_t)cur;
        carry   = cur >> 32;
    }
    if (carry) {
        limbs[*len] = (uint32_t)carry;
        ++(*len);
    }
}

/* bit length of a 32‑bit value (non‑zero).                                   */
static inline size_t bitlen_uint32(uint32_t x)
{
    /* __builtin_clz is undefined for x==0, but we never call it with 0.    */
    return 32 - __builtin_clz(x);
}

/* bit length of a big integer stored little‑endian limb array.               */
static inline size_t bitlen_bigint(const uint32_t *limbs, size_t len)
{
    size_t i = len - 1;
    return i * 32 + bitlen_uint32(limbs[i]);
}

/* Compute the OEIS sequence A025706.                                         */
seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* Estimate required number of bits for 5^n to allocate the limbs array.   */
    /* log2(5) ≈ 2.321928 ≥ 2321/1000, we use 2321/1000 for safety.         */
    size_t bits_est = (n * 2321ULL + 999ULL) / 1000ULL;
    size_t cap_limbs = bits_est / 32 + 2;                 /* add spare space   */
    uint32_t *limbs = (uint32_t *)malloc(cap_limbs * sizeof(uint32_t));
    if (!limbs) return 0;                               /* allocation fail    */
    size_t len = 1;
    limbs[0] = 1;                                       /* 5^0 */

    uint64_t sum_floor = 0; /* accumulates Σ⌊k*c⌋                               */

    for (idx_t k = 1; k <= n; ++k) {
        /* compute 5^k from 5^(k-1) */
        mul_small(limbs, &len, 5);

        /* grow the limb array if necessary */
        if (len > cap_limbs) {
            cap_limbs <<= 1;
            uint32_t *tmp = (uint32_t *)realloc(limbs, cap_limbs * sizeof(uint32_t));
            if (!tmp) {
                free(limbs);
                return 0;
            }
            limbs = tmp;
        }

        /* compute ⌊k * log_4(5)⌋ = ⌊(bitlen(5^k)-1)/2⌋                   */
        size_t bitlen = bitlen_bigint(limbs, len);
        sum_floor += (bitlen - 1) / 2;
    }

    free(limbs);

    /* a(n) = 1 + n + Σ⌊k*c⌋  */
    return (seq_t)(1 + (seq_t)n + (seq_t)sum_floor);
}
