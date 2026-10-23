/*
 * A030317  Write the odd numbers 2n - 1 in base 2 and juxtapose these binary
 *          expansions; read the result bit-by-bit.
 * https://oeis.org/A030317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th bit (1-indexed) of the concatenation of binary
 *            representations of the odd integers starting from 1
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t bitlen_idx(idx_t x)
{
    idx_t len = 0;
    while (x)
    {
        len++;
        x >>= 1;
    }
    return len;
}

seq_t a(idx_t n)
{
    if (n == 0)
    {
        return (seq_t)0;
    }

    idx_t odd = 1;   /* current odd number (2*m-1) */
    idx_t cum = 0;   /* cumulative number of bits seen so far */

    while (1)
    {
        idx_t bl = bitlen_idx(odd);   /* bit length of current odd */
        if (cum + bl >= n)
        {
            idx_t pos   = n - cum - 1;          /* 0‑based position inside this odd */
            idx_t shift = bl - pos - 1;         /* shift to bring target bit to LSB */
            return (seq_t)((odd >> shift) & 1);
        }
        cum += bl;
        odd += 2;   /* move to the next odd number */
    }
}
