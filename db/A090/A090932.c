/*
 * A090932  a(n) = n! / 2^floor(n/2).
 * https://oeis.org/A090932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! / 2^floor(n/2)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) = n! / 2^floor(n/2).
   Uses the recurrence:
     a(0) = a(1) = 1
     a(n) = a(n-2) * ((n-1) * n / 2)   for n >= 2
   This is an O(n) algorithm with only integer arithmetic. */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    seq_t a_nm2 = (seq_t)1;   /* a(i-2)  */
    seq_t a_nm1 = (seq_t)1;   /* a(i-1)  */
    seq_t next, mult;

    for (idx_t i = 2; i <= n; ++i) {
        /* Compute ((i-1) * i) / 2 safely in seq_t. */
        mult = ((seq_t)(i - 1) * (seq_t)i) / (seq_t)2;
        next = a_nm2 * mult;

        /* Shift for next iteration. */
        a_nm2 = a_nm1;
        a_nm1 = next;
    }
    return a_nm1;
}
