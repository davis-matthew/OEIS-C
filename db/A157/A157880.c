/*
 * A157880  Expansion of 136*x^2 / (-x^3+1155*x^2-1155*x+1).
 * https://oeis.org/A157880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A157880: Expansion of 136*x^2 / (-x^3+1155*x^2-1155*x+1)
   Recurrence:
      a(1) = 0
      a(2) = 136
      a(3) = 1155*a(2)
      a(n) = 1155*(a(n-1)-a(n-2)) + a(n-3)   for n > 3   */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle small indices directly.  All arithmetic thereafter
       is performed using seq_t, which may be an arbitrary‑precision
       type; there is no risk of overflow. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)136;

    /* Initialise the first three terms: a1, a2, a3. */
    seq_t a_minus3 = (seq_t)0;                        /* a(1) */
    seq_t a_minus2 = (seq_t)136;                     /* a(2) */
    seq_t a_minus1 = (seq_t)1155 * a_minus2;          /* a(3) */

    if (n == 3) return a_minus1;

    /* Iterate the recurrence from i = 4 up to n. */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t diff  = a_minus1 - a_minus2;    /* a(n-1) - a(n-2)   */
        seq_t next  = diff * (seq_t)1155 + a_minus3;   /* a(n)             */
        a_minus3 = a_minus2;
        a_minus2 = a_minus1;
        a_minus1 = next;
    }
    return a_minus1;
}
