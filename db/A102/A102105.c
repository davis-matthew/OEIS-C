/*
 * A102105  a(n) = (19*5^n - 16*3^n + 1) / 4.
 * https://oeis.org/A102105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (19*5^n - 16*3^n + 1)/4
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 9*a(n-1) - 23*a(n-2) + 15*a(n-3),  a(0)=1, a(1)=12, a(2)=83 */

#include <stdint.h>
#include "../utils/mul_small.h"

#ifndef __SIZEOF_INT128__
#error "__int128 support is required for this implementation"
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)83;

    seq_t a0 = 1;   /* a(n-3) */
    seq_t a1 = 12;  /* a(n-2) */
    seq_t a2 = 83;  /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t t9  = mul_small(a2, 9);   /* 9 * a(n-1) */
        seq_t t23 = mul_small(a1, 23);  /* 23 * a(n-2) */
        seq_t t15 = mul_small(a0, 15);  /* 15 * a(n-3) */
        seq_t next = t9 - t23 + t15;    /* a(n) = 9a(n-1) - 23a(n-2) + 15a(n-3) */

        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
