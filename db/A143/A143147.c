/*
 * A143147  a(1)=2. a(n) = the smallest positive multiple of a(n-1) that is >
 *          a(n-1) and has the same number of 0's as 1's in its binary
 *          representation.
 * https://oeis.org/A143147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 2;
   for n > 1, a(n) is the smallest positive multiple of a(n-1)
   that is strictly larger than a(n-1) and whose binary representation
   contains an equal number of 0’s and 1’s. */

#include <stddef.h>

/* number of bits required to represent x (x > 0) */
static inline idx_t bitlen_seq(seq_t x)
{
    idx_t len = 0;
    while (x)
    {
        ++len;
        x >>= 1;
    }
    return len;
}

/* number of 1 bits in x */
static inline idx_t popcount_seq(seq_t x)
{
    idx_t cnt = 0;
    while (x)
    {
        x &= (x - 1);
        ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* callers will never use n==0, but guard anyway */
        return (seq_t)0;

    seq_t cur = (seq_t)2;   /* a(1) */
    if (n == 1)
        return cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* search for smallest k > 1 such that cur*k works */
        seq_t k = (seq_t)2;
        for (;; ++k)
        {
            seq_t cand = cur * k;
            idx_t bits = bitlen_seq(cand);
            if ((bits & 1) == 0)            /* even number of bits? */
            {
                idx_t ones = popcount_seq(cand);
                if (ones == bits / 2)
                {
                    cur = cand;            /* found a(i) */
                    break;
                }
            }
        }
    }
    return cur;
}
