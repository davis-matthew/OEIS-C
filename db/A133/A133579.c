/*
 * A133579  a(0)=a(1)=1; for n > 1, a(n) = 3*a(n-1) if a(n-1) and n are
 *          coprime, otherwise a(n) = a(n-1)/gcd(a(n-1), n).
 * https://oeis.org/A133579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 49, u32 n <= 106, u64 n <= 223,
 *            u128 n <= 457, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=a(1)=1; for n>1: a(n)=3*a(n-1) if gcd(a(n-1),n)=1 else a(n-1)/gcd(a(n-1),n) */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle trivial cases directly */
    if (n == 0 || n == 1)
        return (seq_t)1;

    seq_t cur = (seq_t)1;      /* a(1) = 1 */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t g = gcd(cur, (seq_t)i);
        if (g == (seq_t)1)
            cur = cur * 3;
        else
            cur = cur / g;
    }
    return cur;
}
