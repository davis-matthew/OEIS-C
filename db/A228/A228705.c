/*
 * A228705  Expansion of (1-2*x+4*x^2-2*x^3+x^4)/((1-x)^4*(1+x^2)^2).
 * https://oeis.org/A228705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 90, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2, a(2)=4, a(3)=10, a(4)=19, a(5)=28,
   a(6)=40, a(7)=60;
   For n ≥ 8: a(n) = 4*a(n-1)-8*a(n-2)+12*a(n-3)-14*a(n-4)+12*a(n-5)
                -8*a(n-6)+4*a(n-7)-a(n-8) */
#include <stdint.h>

seq_t a(idx_t n) {
    static const seq_t init[8] = {
        (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)10,
        (seq_t)19, (seq_t)28, (seq_t)40, (seq_t)60
    };
    if (n < 8) return init[n];

    seq_t a0 = init[0], a1 = init[1], a2 = init[2], a3 = init[3],
          a4 = init[4], a5 = init[5], a6 = init[6], a7 = init[7];

    for (idx_t i = 8; i <= n; ++i) {
        seq_t next = ((seq_t)4 * a7 + (seq_t)12 * a5 + (seq_t)12 * a3 + (seq_t)4 * a1)
                   - ((seq_t)8 * a6 + (seq_t)14 * a4 + (seq_t)8 * a2 + (seq_t)a0);
        a0 = a1; a1 = a2; a2 = a3; a3 = a4; a4 = a5; a5 = a6; a6 = a7; a7 = next;
    }
    return a7;
}
