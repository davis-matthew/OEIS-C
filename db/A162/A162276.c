/*
 * A162276  a(n) = the smallest positive multiple of n such that, when n and
 *          a(n) are written in binary, each has a different number of 1's.
 * https://oeis.org/A162276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest positive multiple m = n*k (k>=2) such that
 *            popcount(m) != popcount(n). Implements a simple search
 * Range:     u8 n <= 29, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Count 1-bits in a seq_t value. */
static unsigned popcount_seq_t(seq_t x)
{
    unsigned c = 0;
    while (x != 0) {
        if ((x & 1) != 0) ++c;
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n==0, guard */

    seq_t seq_n = (seq_t)n;
    unsigned pc_n = popcount_seq_t(seq_n);

    /* start with k = 2; k=1 would give the same popcount as n */
    for (idx_t k = 2;; ++k) {
        seq_t prod = seq_n * (seq_t)k;
        if (popcount_seq_t(prod) != pc_n)
            return prod;
    }
}
