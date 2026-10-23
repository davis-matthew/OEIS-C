/*
 * A154350  a(n) = ( (9 + 2*sqrt(2))^n - (9 - 2*sqrt(2))^n )/(4*sqrt(2)).
 * https://oeis.org/A154350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A154350
   Recurrence: a(0) = 0, a(1) = 1,
   a(n) = 18*a(n-1) - 73*a(n-2)  for n > 1
   (inputs are 1‑based so a(1) is the first term). */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* The sequence is defined by a linear recurrence, so we
       compute it iteratively in O(n) time.  The implementation
       works for any integer build type of seq_t due to the
       restrictions on operations. */
    if (n == 0)          /* safety: not part of OEIS offset */
        return 0;
    if (n == 1)
        return 1;

    seq_t a_prev = 0;   /* a(0) */
    seq_t a_curr = 1;   /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = 18 * a_curr - 73 * a_prev;
        a_prev = a_curr;
        a_curr = next;
    }
    return a_curr;
}
