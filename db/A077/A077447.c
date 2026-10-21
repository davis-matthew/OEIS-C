/*
 * A077447  Numbers k such that (k^2 - 14)/2 is a square.
 * https://oeis.org/A077447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-2) - a(n-4) (n ≥ 5) Initial values: a(1)=4, a(2)=8,
 *            a(3)=16, a(4)=44
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    switch (n) {
        case 1: return (seq_t)4;
        case 2: return (seq_t)8;
        case 3: return (seq_t)16;
        case 4: return (seq_t)44;
    }

    /* iterate using the recurrence. Use 4 sliding registers. */
    seq_t a1 = (seq_t)4;   /* a(n-4) */
    seq_t a2 = (seq_t)8;   /* a(n-3) */
    seq_t a3 = (seq_t)16;  /* a(n-2) */
    seq_t a4 = (seq_t)44;  /* a(n-1) */
    seq_t next;

    for (idx_t i = 5; i <= n; ++i) {
        next = (seq_t)6 * a3 - a1;  /* a(i) = 6*a(i-2) - a(i-4) */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;
}
