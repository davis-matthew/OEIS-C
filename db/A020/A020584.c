/*
 * A020584  Expansion of 1/((1-6*x)*(1-8*x)*(1-10*x)).
 * https://oeis.org/A020584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A020584 – Expansion of 1/((1-6*x)*(1-8*x)*(1-10*x))
   Formula: a(n) = (9*6^n)/2 - 16*8^n + (25*10^n)/2,
   Recurrence (valid for n ≥ 3):
       a(n) = 18*a(n-1) - 80*a(n-2) + 6^n
   Initial values: a(0)=1, a(1)=24, a(2)=388
*/
#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)24;
    if (n == 2) return (seq_t)388;

    /* We'll compute iteratively using the recurrence
       a(n) = 18*a(n-1) - 80*a(n-2) + 6^n
       We keep the previous two values and the current pow6. */
    seq_t a_m2 = (seq_t)24;   /* a(1) */
    seq_t a_m1 = (seq_t)388;  /* a(2) */
    seq_t pow6  = (seq_t)36;  /* 6^2, will be multiplied at i=3 */

    for (idx_t i = 3; i <= n; ++i) {
        pow6 *= (seq_t)6;                 /* 6^i */
        seq_t cur = (seq_t)18 * a_m1;     /* 18*a(n-1)  */
        cur = cur - (seq_t)80 * a_m2;     /* -80*a(n-2) */
        cur = cur + pow6;                 /* + 6^n      */
        a_m2 = a_m1;                      /* shift window */
        a_m1 = cur;
    }
    return a_m1;
}
