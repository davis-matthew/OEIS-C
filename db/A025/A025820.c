/*
 * A025820  Expansion of 1/((1-x^2)*(1-x^7)*(1-x^12)).
 * https://oeis.org/A025820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A025820: partitions of n into parts 2, 7 and 12.
   Implements the recurrence
      a(0) = 1,
      a(n) = a(n-2) + a(n-7) - a(n-9) + a(n-12) - a(n-14)
             - a(n-19) + a(n-21)   for n ≥ 1
   (terms with negative indices are treated as 0).  The recurrence
   only depends on the 21 previous values, so a ring buffer of size
   22 suffices for all n.  This keeps the algorithm O(n) in time
   and O(1) in memory, works for any order of calls, and is safe
   for both fixed‑width and arbitrary‑precision seq_t. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ring buffer that stores the last 22 computed values */
    static seq_t buf[22];
    /* initialize buffer to zero on first entry */
    static int init_done = 0;
    if (!init_done) {
        for (size_t i = 0; i < 22; ++i)
            buf[i] = 0;
        init_done = 1;
    }

    /* a(0) = 1 */
    buf[0] = 1;
    if (n == 0)
        return buf[0];

    for (idx_t i = 1; i <= n; ++i) {
        seq_t val = 0;
        if (i >= 2)  val += buf[(i - 2) % 22];
        if (i >= 7)  val += buf[(i - 7) % 22];
        if (i >= 9)  val -= buf[(i - 9) % 22];
        if (i >= 12) val += buf[(i - 12) % 22];
        if (i >= 14) val -= buf[(i - 14) % 22];
        if (i >= 19) val -= buf[(i - 19) % 22];
        if (i >= 21) val += buf[(i - 21) % 22];
        buf[i % 22] = val;
    }
    return buf[n % 22];
}
