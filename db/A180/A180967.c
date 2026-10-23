/*
 * A180967  Number of n-game win/loss series that contain at least one dead
 *          game.
 * https://oeis.org/A180967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n - 2*C(n-1,(n-1)/2) for odd n; * a(n) = 2^n -
 *            4*C(n-1,n/2) for even n
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 1u) return 0;          /* safety for non‑positive input */
    seq_t total = pow2(n);         /* 2^n                           */
    seq_t sub;

    if (n & 1u) {                  /* odd n                          */
        sub = binom(n - 1, (n - 1) / 2);
        sub = sub * (seq_t)2;      /* multiply by 2                  */
    } else {                       /* even n                         */
        sub = binom(n - 1, n / 2);
        sub = sub * (seq_t)4;      /* multiply by 4                  */
    }

    return total - sub;            /* final count                    */
}
