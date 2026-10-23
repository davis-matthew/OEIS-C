/*
 * A145715  Numbers X such that there exists Y in N with X^2 = 381*Y^2 + 127.
 * https://oeis.org/A145715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A145715: a(1)=254, a(2)=515366
   Recurrence: a(n) = 2030 * a(n-1) - a(n-2)  for n >= 3   */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* undefined for n < 1, return 0 */

    if (n == 1) return (seq_t)254;
    if (n == 2) return (seq_t)515366;

    seq_t a_prevprev = (seq_t)254;  /* a(n-2) */
    seq_t a_prev     = (seq_t)515366;  /* a(n-1) */
    seq_t a_curr;

    for (idx_t i = 3; i <= n; ++i) {
        a_curr = ((seq_t)2030) * a_prev - a_prevprev;
        a_prevprev = a_prev;
        a_prev = a_curr;
    }
    return a_prev;   /* a(n) */
}
