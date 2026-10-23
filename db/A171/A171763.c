/*
 * A171763  Odd numbers whose binary expansion begins 10.
 * https://oeis.org/A171763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A171763: odd numbers whose binary expansion begins 10.
 * For n >= 1, let k = ceil(log2(n + 1)) + 2.
 * The nth term is
 *     a(n) = 2^(k-1) + ( (n - 2^(k-3)) << 1 ) + 1
 *
 * The index n is 1‑based and the first term a(1) = 5.
 */

#include <stdint.h>
#include "../utils/floor_log2_uint64.h"   /* floor_log2_uint64(idx_t v) */
#include "../utils/pow2.h"                /* pow2(idx_t exp) returns seq_t */

static unsigned ceil_log2_u64(idx_t v) {
    /* v >= 1 */
    unsigned fl = floor_log2_uint64(v);
    return (((idx_t)1 << fl) == v) ? fl : fl + 1;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* not needed, safety */
    idx_t xp1 = n + 1;                        /* n+1 >= 2 */
    unsigned ceil = ceil_log2_u64(xp1);       /* ceil(log2(n+1)) */
    unsigned k = ceil + 2;                    /* bit length of the term */

    idx_t block_start = (idx_t)1 << (k - 3);   /* first index of this block */
    idx_t d = n - block_start;                /* offset within the block (0‑based) */

    seq_t result = pow2(k - 1) + ((seq_t)d << 1) + 1;
    return result;
}
