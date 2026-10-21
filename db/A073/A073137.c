/*
 * A073137  a(n) is the least number whose binary representation has the same
 *          number of 0's and 1's as n.
 * https://oeis.org/A073137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(A000120(n)-1)) * (2^A023416(n)+1) - 1
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A000120(n) = number of 1‑bits in n, A023416(n) = number of 0‑bits in the binary representation of n */

#include "../utils/pow2.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* number of 1‑bits in n */
    idx_t ones = __builtin_popcountll((unsigned long long)n);

    /* total number of bits needed to represent n (>=1) */
    idx_t bits = (idx_t)floor_log2_uint64(n) + 1;

    /* number of 0‑bits in the binary representation of n */
    idx_t zeros = bits - ones;

    /* 2^(ones-1) and 2^zeros */
    seq_t p1 = pow2(ones - 1);
    seq_t p0 = pow2(zeros);

    /* a(n) = p1 * (p0 + 1) - 1 */
    seq_t result = p1 * (p0 + (seq_t)1) - (seq_t)1;
    return result;
}
