/*
 * A143146  a(n) is the smallest positive multiple of n that has the same
 *          number of 0's as 1's in its binary representation.
 * https://oeis.org/A143146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n * A351599(n) where A351599(n) is the smallest k ≥ 1
 *            such that the product n·k has an equal number of 0-bits and
 *            1-bits in its binary representation
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool, allowed but not required */
#include "../utils/bitlen_local.h"

/* Count the number of set bits (ones) in a seq_t value. */
static inline unsigned popcount_seq_t(seq_t x)
{
    unsigned cnt = 0;
    while (x)
    {
        cnt += (unsigned)(x & 1);
        x >>= 1;
    }
    return cnt;
}

/* Computes the sequence value a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset 1 means n≥1; safeguard */

    const seq_t num = (seq_t)n;         /* convert index to seq_t */
    seq_t k = 1;                        /* multiplier candidate */

    for (;; ++k)
    {
        seq_t x = num * k;              /* candidate multiple */

        unsigned bitlen = bitlen_local(x);
        unsigned ones   = popcount_seq_t(x);

        if (2 * ones == bitlen)         /* equal zeros and ones */
            return x;
    }
}
