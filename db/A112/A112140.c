/*
 * A112140  (a(2n)+a(2n+1))^2 = a(2n) + 10000*a(2n+1).
 * https://oeis.org/A112140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A112140
   (a(2n)+a(2n+1))^2 = a(2n) + 10000*a(2n+1)
   The n-th pair (x=a(2k), y=a(2k+1)) is obtained from
   s = residue[k mod 8] + 9999*(k/8),
   y = (s*(s-1))/9999,
   x = s - y
   where residue[] = {0,1,2223,2728,4950,5050,7272,7777}
*/

#include <stdlib.h>

static const seq_t residue[8] = {
    (seq_t)0,
    (seq_t)1,
    (seq_t)2223,
    (seq_t)2728,
    (seq_t)4950,
    (seq_t)5050,
    (seq_t)7272,
    (seq_t)7777
};

seq_t a(idx_t n)
{
    /* pair index (0‑based) */
    idx_t k = n / 2;

    /* residue index and block counter */
    idx_t ridx = k % 8;
    idx_t t    = k / 8;

    /* compute s = residue + 9999*t */
    seq_t s = residue[ridx] + (seq_t)9999 * (seq_t)t;

    if (s == 0)          /* special case for pair (0,0) */
        return (n & 1) ? (seq_t)0 : (seq_t)0;

    /* compute y = (s*(s-1))/9999 avoiding overflow */
    seq_t s_div  = s / 9999;
    seq_t s_mod  = s % 9999;
    seq_t y = s_div * (s - 1) + (s_mod * (s - 1)) / 9999;
    seq_t x = s - y;

    return (n & 1) ? y : x;
}
