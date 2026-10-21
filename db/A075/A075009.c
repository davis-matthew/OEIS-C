/*
 * A075009  Floor[ concatenation of 8 numbers from n+7 to n in that order
 *          divided by 8].
 * https://oeis.org/A075009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor( concat(n+7, n+6, …, n) / 8 )
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 97, u128 n <= 1000,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  concat is the decimal concatenation of the 8 successive numbers
    in the order n+7, n+6, …, n.  The quotient is computed
    incrementally so that only the current remainder modulo 8
    and the partial quotient are kept. */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t q = 0;          /* partial quotient */
    idx_t  r = 0;          /* remainder modulo 8 */

    idx_t i = n + 7;       /* start from the largest term */
    for (;;) {
        /* decimal digits of the current number i */
        idx_t x = i;
        idx_t d = 1;
        while (x >= 10) {
            x /= 10;
            d++;
        }

        /* 10^d as seq_t, may be > 64‑bit on big‑int builds */
        seq_t pow10d = pow10(d);

        /* compute (r * 10^d + i) as seq_t; r < 8, so this stays small */
        seq_t t   = (seq_t)r * pow10d + (seq_t)i;

        /* increment in the quotient and the new remainder */
        seq_t q_inc = t / (seq_t)8;
        r = (idx_t)(t % (seq_t)8);

        /* update the quotient: q * 10^d + q_inc */
        q = q * pow10d + q_inc;

        if (i == n)     /* finished the last of the 8 numbers */
            break;
        i--;            /* next number downwards */
    }
    return q;
}
