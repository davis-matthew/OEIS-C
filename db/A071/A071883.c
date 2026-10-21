/*
 * A071883  A002487(n)*A002487(n+2).
 * https://oeis.org/A071883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002487(n) * A002487(n+2) where A002487(n) is the
 *            numerator of the nth term of the Calkin–Wilf sequence
 *            (Calkin-Wilf tree numerators). A002487(n) for n≥1 is computed
 *            from the binary expansion of n as follows: start a=1, b=1. for
 *            each bit of n after the most significant bit (going from high...
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   bitlen_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bitlen_idx.h"   /* provides unsigned int bitlen_idx(idx_t); */

static seq_t calkin_wilf_numerator(idx_t n)
{
    if (n == 0)
        return 0;                      /* by convention for the product */
    seq_t a = 1, b = 1;
    unsigned int len = bitlen_idx(n);  /* number of bits in n */
    /* process bits from the second most significant down to 0 */
    for (int i = (int)len - 2; i >= 0; --i)
    {
        if ((n >> i) & 1U)
            a += b;     /* right */
        else
            b += a;     /* left */
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t fn   = calkin_wilf_numerator(n);
    seq_t fn2  = calkin_wilf_numerator(n + 2);
    return fn * fn2;
}
