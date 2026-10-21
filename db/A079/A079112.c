/*
 * A079112  Numbers in binary representation with odd length.
 * https://oeis.org/A079112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   bitlen_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A079112: Numbers whose binary representation has odd length.
   a(0) = 0
   For n ≥ 1, a(n) is the decimal number obtained by interpreting
   the binary representation of the n‑th integer with an odd number of
   bits as a base‑10 integer.
   The implementation finds the corresponding odd bit‑length block,
   builds the binary digits, and converts them to decimal without
   ever creating the binary number itself. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bitlen_idx.h"

/* External linkage */
seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 0 by definition */
        return (seq_t)0;

    /* count = number of terms already accounted for in previous blocks */
    idx_t count = 0;
    /* pow2 = 2^(L-1).  When L is odd, pow2 is exactly the amount of
       numbers of that block, and its bit‑length equals L. */
    idx_t pow2 = 1;

    for (;;)
    {
        idx_t block = pow2;                 /* numbers with current odd length */

        if (n <= count + block)             /* n lies in this block ? */
        {
            idx_t offset = n - count - 1;   /* 0‑based position inside block */

            unsigned len = bitlen_idx(pow2);/* odd length L (bitlen(2^(L-1)) = L) */
            seq_t  res  = 1;                /* most significant binary digit = 1 */

            /* process the remaining L‑1 bits of the binary representation */
            for (unsigned pos = 1; pos < len; ++pos)
            {
                unsigned bit = (unsigned)((offset >> (len - 1 - pos)) & 1);
                res = res * 10 + (seq_t)bit;
            }
            return res;
        }

        /* skip this block and move to the next odd bit‑length (multiply pow2 by 4) */
        count += block;
        pow2 <<= 2;   /* next odd L: 2^(L+2-1) = 4 * 2^(L-1) */
    }
}
