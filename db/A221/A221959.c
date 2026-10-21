/*
 * A221959  Expansion of (2-17*x)/((1-4*x)*(1-6*x)*(1-8*x)*(1-10*x)*(1-16*x)).
 * https://oeis.org/A221959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 44·a(n-1) – 732·a(n-2) + 5776·a(n-3) – 21632·a(n-4) +
 *            30720·a(n-5), with a(0)=2, a(1)=71, a(2)=1660, a(3)=32620,
 *            a(4)=586992
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)71;
    if (n == 2) return (seq_t)1660;
    if (n == 3) return (seq_t)32620;
    if (n == 4) return (seq_t)586992;

    seq_t a0 = (seq_t)2;
    seq_t a1 = (seq_t)71;
    seq_t a2 = (seq_t)1660;
    seq_t a3 = (seq_t)32620;
    seq_t a4 = (seq_t)586992;
    seq_t next;

    for (idx_t i = 5; i <= n; ++i) {
        next = mul_small(a4, 44)                     /* 44·a(n-1)    */
             - mul_small(a3, 732)                     /* -732·a(n-2)  */
             + mul_small(a2, 5776)                    /* +5776·a(n-3) */
             - mul_small(a1, 21632)                   /* -21632·a(n-4)*/
             + mul_small(a0, 30720);                  /* +30720·a(n-5)*/

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
