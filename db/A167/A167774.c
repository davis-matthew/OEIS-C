/*
 * A167774  Subsequence of A167708 whose indices are congruent to 1 mod 5,
 *          i.e., a(n) = A167708(5*n+1).
 * https://oeis.org/A167774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 340*a(n-1) - a(n-2); a(0)=9; a(1)=1530
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the nth term of OEIS sequence A167774.
   The sequence satisfies the linear recurrence
   a(n+2) = 340*a(n+1) - a(n)  with a(0)=9 and a(1)=1530. */
seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) {
        return (seq_t)9;
    }
    if (n == 1) {
        return (seq_t)1530;
    }

    /* Iterate the recurrence.  Using two temporary variables
       avoids the need for big‑integer array storage and keeps
       the algorithm O(n) with minimal memory usage. */
    seq_t a_prev = (seq_t)9;      /* a(0)   */
    seq_t a_curr = (seq_t)1530;   /* a(1)   */
    seq_t a_next;

    /* Use idx_t counter; compare with n+1 to avoid overflow
       if n were extremely large (though the problem limits to ~10000). */
    for (idx_t i = 2; i <= n; ++i) {
        a_next = (seq_t)340 * a_curr - a_prev;
        a_prev = a_curr;
        a_curr = a_next;
    }

    return a_curr;
}
