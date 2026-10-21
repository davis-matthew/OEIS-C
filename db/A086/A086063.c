/*
 * A086063  a(n) = if n<=1 then 1 else smallest k>1 such that in binary
 *          representation n is contained in n^k.
 * https://oeis.org/A086063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = if n<=1 then 1 else smallest k>1 such that in binary
 *            representation n is contained in n^k
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_big_by_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n). */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mul_big_by_uint64.h"

static inline int bitlen_uint64(uint64_t x) {
    if (x == 0) return 0;
    return 64 - __builtin_clzll(x);
}

/* Compute the bit length of a big integer (array of 64‑bit limbs). */
static size_t bitlen_big(const uint64_t *limbs, size_t len) {
    if (len == 0) return 0;
    size_t i = len - 1;
    while (i > 0 && limbs[i] == 0) --i;
    if (limbs[i] == 0) return 0;
    return i * 64 + bitlen_uint64(limbs[i]);
}

/* Return the value of the pattern (a uint64_t) and its length L. */
static inline void get_pattern(idx_t n, uint64_t *pat, size_t *L) {
    uint64_t x = (uint64_t)n;
    *pat = x;
    *L   = bitlen_uint64(x);
    if (*L == 0) *L = 1;   /* for the case n==0, though this is handled earlier */
}

/* Extract L bits starting at bit position 'shift' from the big integer 'limbs'. */
static uint64_t extract_bits(const uint64_t *limbs, size_t len,
                             size_t shift, size_t L) {
    size_t limb = shift >> 6;          /* shift / 64 */
    uint32_t offset = shift & 63;      /* shift % 64 */

    if (offset + L <= 64) {             /* bits lie within one limb */
        uint64_t val = limbs[limb] >> offset;
        if (L < 64)
            val &= ((uint64_t)1 << L) - 1;
        return val;
    } else {                            /* bits cross a limb boundary */
        uint64_t low  = limbs[limb] >> offset;
        uint64_t high = 0;
        if (limb + 1 < len)
            high = limbs[limb + 1] << (64 - offset);
        uint64_t val = low | high;
        if (L < 64)
            val &= ((uint64_t)1 << L) - 1;
        return val;
    }
}

seq_t a(idx_t n) {
    if (n <= 1) return 1;   /* a(0) = a(1) = 1 */

    uint64_t pattern;
    size_t L;
    get_pattern(n, &pattern, &L);

    /* Big integer representation of n^k.  Start with n^1.  */
    uint64_t big[256] = {0};     /* enough for n up to 10 000 and reasonable k */
    size_t len = 1;
    big[0] = (uint64_t)n;        /* n^1 */

    for (seq_t k = 2; ; ++k) {   /* we search for k > 1 */
        /* compute n^k = n^(k-1) * n */
        mul_big_by_uint64(big, &len, (uint64_t)n);

        size_t M = bitlen_big(big, len);
        if (M < L) continue;    /* not enough bits to contain the pattern */

        size_t max_shift = M - L;
        for (size_t shift = 0; shift <= max_shift; ++shift) {
            if (extract_bits(big, len, shift, L) == pattern)
                return k;
        }
    }
}
