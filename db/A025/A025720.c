/*
 * A025720  Index of 7^n within the sequence of the numbers of the form
 *          2^i*7^j.
 * https://oeis.org/A025720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + sum_{k=1..n} floor(k*log_2(7)) + n (derived from a(n)
 *            = 1 + Σ_{k=0..n} ceil(k*log_2(7)), with ceil(0)=0 and for k>0
 *            ceil(kc)=floor(kc)+1)
 * Range:     u8 n <= 12, u16 n <= 215, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* compute floor of a 32‑bit unsigned integer in binary */
static inline int bitlen_uint32(uint32_t x)
{
    int n = 0;
    while (x) { x >>= 1; ++n; }
    return n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    /* big integer representing 7^k, base 2^32, little endian limbs */
    size_t cap   = 1;
    size_t len   = 1;
    uint32_t *limbs = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!limbs) return (seq_t)0;          /* allocation failure not expected */
    limbs[0] = 1;                         /* 7^0 = 1 */

    seq_t sum_t = 0;                      /* Σ floor(k*log2(7)) for k=1..n */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* multiply big integer by 7 */
        uint64_t carry = 0;
        for (size_t i = 0; i < len; ++i) {
            uint64_t prod = (uint64_t)limbs[i] * 7 + carry;
            limbs[i] = (uint32_t)prod;
            carry = prod >> 32;
        }
        if (carry) {
            if (len == cap) {
                cap *= 2;
                uint32_t *p = (uint32_t *)realloc(limbs, cap * sizeof(uint32_t));
                if (!p) { free(limbs); return (seq_t)0; }
                limbs = p;
            }
            limbs[len] = (uint32_t)carry;
            ++len;
        }

        /* compute floor(k*log2(7)) = bitlen(7^k) - 1 */
        uint32_t ms = limbs[len-1];
        int msbits = bitlen_uint32(ms);          /* number of bits in most significant limb */
        seq_t t_k = (seq_t)((len - 1) * 32 + msbits - 1);
        sum_t += t_k;
    }

    seq_t result = 1 + sum_t + (seq_t)n;        /* a(n) = 1 + Σ floor(k*log2(7)) + n */
    free(limbs);
    return result;
}
