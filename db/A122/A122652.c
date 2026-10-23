/*
 * A122652  a(0) = 0, a(1) = 4; for n > 1, a(n) = 10*a(n-1) - a(n-2).
 * https://oeis.org/A122652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 4; for n > 1, a(n) = 10*a(n-1) - a(n-2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* External linkage is required. */
seq_t a(idx_t n)
{
    /* Handle the two base values directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)4;

    /* a_prev will hold a(n-1), a_cur will hold a(n).  Start with
       a(1) and a(0).  We keep only two values in memory, so the
       algorithm requires O(n) time but O(1) extra space. */
    seq_t a_prev = (seq_t)0;
    seq_t a_cur  = (seq_t)4;
    seq_t a_next;

    /* Iterate from 2 up to n, computing each term from the
       recurrence.  The multiplication 10*a_cur does not overflow
       for all terms that fit within seq_t. */
    for (idx_t i = 2; i <= n; ++i)
    {
        a_next = (seq_t)10 * a_cur - a_prev;
        a_prev = a_cur;
        a_cur  = a_next;
    }

    /* a_cur now holds a(n). */
    return a_cur;
}
