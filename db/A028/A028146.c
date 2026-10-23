/*
 * A028146  Expansion of 1/((1-4x)(1-7x)(1-8x)(1-10x)).
 * https://oeis.org/A028146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*10^(n+3) - 9*8^(n+3) + 8*7^(n+3) - 4^(n+3))/72
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following implementation uses the recurrence derived from the
 * generating function 1/((1-4x)(1-7x)(1-8x)(1-10x)):
 *   a(n) = 29*a(n-1) - 306*a(n-2) + 1384*a(n-3) - 2240*a(n-4)
 * with initial values a(0)=1, a(1)=29, a(2)=535, a(3)=8025.  */
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)29;
    if (n == 2) return (seq_t)535;
    if (n == 3) return (seq_t)8025;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)29;
    seq_t a2 = (seq_t)535;
    seq_t a3 = (seq_t)8025;
    seq_t a_next;

    for (idx_t i = 4; i <= n; ++i) {
        a_next = (seq_t)29 * a3;
        a_next -= (seq_t)306 * a2;
        a_next += (seq_t)1384 * a1;
        a_next -= (seq_t)2240 * a0;

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a_next;
    }
    return a3;
}
