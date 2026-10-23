/*
 * A147761  a(n) is the smallest positive integer m with exactly n zeros in
 *          its binary representation and with n represented in binary as a
 *          substring of the binary representation of m.
 * https://oeis.org/A147761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n*2^(n-A023416(n))
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 60, u128 n <= 122,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A023416(n) = number of zeros in the binary representation of n
 * after removing the leading 1, i.e. A023416(n) = bitlen(n) - popcount(n).
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"   /* floor_log2_uint64() */

/* count the 1‑bits of an idx_t */
static inline idx_t popcount_idx(idx_t x)
{
    idx_t c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

/* return 2^exp as a seq_t (safe for exp up to any size that fits in seq_t) */
static seq_t pow2_exp(idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r += r;        /* double r: r = r * 2 */
    return r;
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1; behavior below offset is irrelevant */
        return 0;

    /* number of bits in n (including the leading 1) */
    idx_t bits = floor_log2_uint64(n) + 1;

    /* number of 1’s in n */
    idx_t ones = popcount_idx(n);

    /* A023416(n) = bits - ones */
    idx_t zeros = bits - ones;

    /* exponent for the power of two */
    idx_t exp = n - zeros;

    seq_t val = pow2_exp(exp);
    return (seq_t)n * val;
}
