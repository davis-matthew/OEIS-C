/*
 * A037084  Positive integers not going to 1 under iterations of the map in
 *          A001281: n->3n-1 if n odd, n->n/2 if n even.
 * https://oeis.org/A037084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1027, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 79 terms through n = 8584 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A037084: positive integers that never reach 1 under
   the iteration f(n) = 3*n-1 if n is odd, n/2 if n is even. */

#include <stdbool.h>

/* Return true if the iterative map eventually reaches 1,
   false if it enters a cycle that does not contain 1.
   The algorithm uses Floyd’s cycle–detection method,
   but checks for reaching 1 after each individual step. */
static bool reaches_one(seq_t n)
{
    seq_t one = (seq_t)1;
    seq_t slow = n, fast = n;

    while (1) {
        /* Immediate hit of 1 */
        if (slow == one) return true;
        if (fast == one) return true;

        /* Advance slow by one step */
        if (slow % (seq_t)2 == 0)
            slow = slow / (seq_t)2;
        else
            slow = (seq_t)3 * slow - one;
        if (slow == one) return true;

        /* Advance fast by two steps, checking after each step */
        if (fast % (seq_t)2 == 0)
            fast = fast / (seq_t)2;
        else
            fast = (seq_t)3 * fast - one;
        if (fast == one) return true;

        if (fast % (seq_t)2 == 0)
            fast = fast / (seq_t)2;
        else
            fast = (seq_t)3 * fast - one;
        if (fast == one) return true;

        /* If slow and fast coincide, a cycle that does not contain 1
           has been found. */
        if (slow == fast) return false;
    }
}

/* Main sequence function.  a(1) is the first positive integer that does
   not reach 1 under the given map. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, this case is unused */

    idx_t count = 0;
    seq_t candidate = (seq_t)1;

    while (1) {
        if (!reaches_one(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
        ++candidate;
    }
}
