/*
 * A060647  Number of alpha-beta evaluations in a tree of depth n and
 *          branching factor b=3.
 * https://oeis.org/A060647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 38, u64 n <= 79, u128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: OEIS A060647
   Formula: a(2k)   = 2*3^k  - 1
            a(2k+1) = 3^k + 3^(k+1) - 1
   where k = floor(n/2).  This directly follows the definition
   of the number of alpha‑beta evaluations in a full ternary tree
   of depth n.
*/

#include <stdint.h>

static seq_t pow3(idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) {
        res *= (seq_t)3;
    }
    return res;
}

seq_t a(idx_t n) {
    if ((n & 1) == 0) {                /* n is even: n = 2k */
        idx_t k = n >> 1;
        seq_t p = pow3(k);             /* p = 3^k */
        return (seq_t)2 * p - (seq_t)1;
    } else {                           /* n is odd: n = 2k+1 */
        idx_t k = n >> 1;              /* (n-1)/2 */
        seq_t pk = pow3(k);            /* 3^k */
        seq_t pk1 = pk * (seq_t)3;     /* 3^(k+1) */
        return pk + pk1 - (seq_t)1;
    }
}
