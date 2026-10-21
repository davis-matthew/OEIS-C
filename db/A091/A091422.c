/*
 * A091422  Write the natural numbers in binary, break the result up into
 *          blocks of 2 digits each and convert each block back into decimal.
 * https://oeis.org/A091422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = decimal value of the 2‑bit block starting at bit‑index
 *            2*(n‑1) in the concatenation of binary representations of
 *            1,2,3,…
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for offset 0, guard */
    idx_t target = 2 * n;                 /* number of bits to read */
    seq_t val = 0;                        /* accumulator – keep only last two bits */
    idx_t count = 0;                      /* bits processed so far */

    for (idx_t num = 1; count < target; ++num)
    {
        idx_t len = floor_log2_uint64(num) + 1;   /* bit length of num */

        for (idx_t bit = 0; bit < len && count < target; ++bit)
        {
            int b = (num >> (len - 1 - bit)) & 1;     /* next bit of num */
            val = ((val << 1) | (seq_t)b) & (seq_t)3; /* keep last two bits only */
            ++count;
        }
    }
    return val;
}
