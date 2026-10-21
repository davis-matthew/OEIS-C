/*
 * A062954  Least common multiple of Lucas numbers L(0), L(1), ..., L(n).
 * https://oeis.org/A062954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = lcm(L(0), L(1), …, L(n)) where L(0)=2, L(1)=1 and
 *            L(k)=L(k-1)+L(k-2) for k≥2 (least common multiple of the Lucas
 *            numbers up to index n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* L(0)=2, L(1)=1.  Handle the base cases directly. */
    if (n == (idx_t)0 || n == (idx_t)1)
        return (seq_t)2;

    /* Current Lucas numbers. */
    seq_t L_prev = (seq_t)2;   /* L(0) */
    seq_t L_cur  = (seq_t)1;   /* L(1) */

    /* Current lcm of L(0)…L(1).  L(0)=2, L(1)=1 → lcm=2. */
    seq_t lcm = (seq_t)2;

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        /* Next Lucas number. */
        seq_t L_next = L_prev + L_cur;

        /* Update the lcm with the new Lucas number. */
        seq_t g = gcd(lcm, L_next);       /* gcd(lcm, L_next) */
        lcm = (lcm / g) * L_next;         /* lcm(lcm, L_next) */

        /* Shift Lucas numbers for next iteration. */
        L_prev = L_cur;
        L_cur  = L_next;
    }
    return lcm;
}
