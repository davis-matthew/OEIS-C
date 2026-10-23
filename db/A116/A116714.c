/*
 * A116714  Number of permutations of length n that avoid the patterns 321,
 *          1342, 4123.
 * https://oeis.org/A116714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 36, u64 n <= 72, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A116714
   Recurrence: a(n) = 2*a(n-1) - a(n-4)  for n ≥ 7
   Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=5,
                    a(4)=12, a(5)=24, a(6)=46   */

/* The implementation uses only constant space and integer arithmetic,
   so it works for any supported seq_t including arbitrary‑precision types. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Small ring buffer of size 7 holding the last computed values. */
    static const seq_t init[7] = {
        (seq_t)1, /* a(0) */
        (seq_t)1, /* a(1) */
        (seq_t)2, /* a(2) */
        (seq_t)5, /* a(3) */
        (seq_t)12,/* a(4) */
        (seq_t)24,/* a(5) */
        (seq_t)46 /* a(6) */
    };

    seq_t ring[7];
    /* Copy the initial seven values into the ring buffer. */
    for (idx_t i = 0; i < 7; ++i)
        ring[i] = init[i];

    if (n < 7)
        return ring[n];

    /* Compute a(n) using the recurrence, overwriting the ring buffer
       entries as we go.  ring[i % 7] will hold a(i). */
    for (idx_t i = 7; i <= n; ++i) {
        idx_t idx_prev = (i - 1) % 7;     /* a(i-1) */
        idx_t idx_prod = (i - 4) % 7;     /* a(i-4) */
        ring[i % 7] = 2 * ring[idx_prev] - ring[idx_prod];
    }

    return ring[n % 7];
}
