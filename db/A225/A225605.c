/*
 * A225605  a(1) = least k such that 1/3 < H(k) - 1/3; a(2) = least k such
 *          that H(a(1)) - H(3) < H(k) - H(a(1)), and for n > 2, a(n) = least
 *          k such that H(a(n-1)) - H(a(n-2)) > H(k) - H(a(n-1)), where H =
 *          harmonic number.
 * https://oeis.org/A225605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 34, u64 n <= 71, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n‑1) – a(n‑4);
   Initial values: a(1)=5, a(2)=9, a(3)=16, a(4)=29. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* not defined, safe guard */
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)16;
    if (n == 4) return (seq_t)29;

    seq_t a1 = (seq_t)5;   /* a(i-4)   */
    seq_t a2 = (seq_t)9;   /* a(i-3)   */
    seq_t a3 = (seq_t)16;  /* a(i-2)   */
    seq_t a4 = (seq_t)29;  /* a(i-1)   */
    seq_t next = 0;

    for (idx_t i = 5; i <= n; ++i) {
        next = (seq_t)(2 * a4 - a1);
        /* shift window */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return next;
}
