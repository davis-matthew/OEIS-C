/*
 * A014260  Iccanobif numbers: add a(n-1) to reversal of a(n-2).
 * https://oeis.org/A014260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 0, a(1) = 1, a(n) = a(n-1) + rev_decimal(a(n-2)) for n >=
 *            2. rev_decimal(…) reverses the decimal digits of its argument
 * Range:     u8 n <= 13, u16 n <= 19, u32 n <= 36, u64 n <= 68, u128 n <= 132,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    /* Handle the initial instances directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* t0 and t1 keep the two most recent values of the sequence. */
    seq_t t0 = (seq_t)0;          /* a(n-2) */
    seq_t t1 = (seq_t)1;          /* a(n-1) */
    seq_t t2;                      /* will become a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t rev = reverse_decimal(t0); /* rev_decimal(a(n-2)) */
        t2 = t1 + rev;                   /* a(n) */
        t0 = t1;                         /* shift window */
        t1 = t2;
    }
    return t1;   /* t1 holds a(n) after the loop finishes */
}
