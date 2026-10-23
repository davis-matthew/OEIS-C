/*
 * A157409  Minimum of { k > 0 : [2^n / 3^k] mod 6 = 3 } if such k exists, 0
 *          otherwise.
 * https://oeis.org/A157409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A157409.
   a(n) = minimal k > 0 such that floor(2^n / 3^k) mod 6 = 3,
   or 0 if no such k exists. */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t pow2_n = pow2(n);          /* 2^n */
    seq_t pow3_k = (seq_t)3;        /* 3^1 initially */
    idx_t   k     = 1;              /* current k */

    /* If 3^k already exceeds 2^n we can stop immediately. */
    while (pow3_k <= pow2_n)
    {
        seq_t q = pow2_n / pow3_k;   /* floor(2^n / 3^k)       */
        if ((q % (seq_t)6) == (seq_t)3)
            return (seq_t)k;         /* found minimal k */

        /* advance to next power of 3 : k++ */
        pow3_k *= (seq_t)3;
        k++;
    }
    return (seq_t)0;                /* no such k */
}
