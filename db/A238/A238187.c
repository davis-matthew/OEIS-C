/*
 * A238187  Gaussian norm of 1+(1+i)^n.
 * https://oeis.org/A238187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + (1+i)^n + (1-i)^n + 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pow2n   = seq_pow((seq_t)2, n);   /* 2^n */
    seq_t result  = pow2n + (seq_t)1;       /* 2^n + 1 */

    idx_t r = n % 8;
    switch (r) {
        case 0: {
            seq_t term = seq_pow((seq_t)2, n / 2 + 1);  /* + 2^(n/2+1) */
            result += term;
            break;
        }
        case 1:
        case 7: {
            seq_t term = seq_pow((seq_t)2, (n + 1) / 2);  /* + 2^((n+1)/2) */
            result += term;
            break;
        }
        case 2:
        case 6:
            /* term is zero; nothing to do */
            break;
        case 3:
        case 5: {
            seq_t term = seq_pow((seq_t)2, (n + 1) / 2);  /* - 2^((n+1)/2) */
            result -= term;
            break;
        }
        case 4: {
            seq_t term = seq_pow((seq_t)2, n / 2 + 1);  /* - 2^(n/2+1) */
            result -= term;
            break;
        }
    }
    return result;
}
