/*
 * A209041  Number of n X 2 0..3 arrays with no element equal to the average
 *          of its immediate neighbors vertically above and horizontally left.
 * https://oeis.org/A209041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A209041:
   a(1)=12, a(2)=124, a(3)=1300
   for n ≥ 4: a(n) = 11*a(n-1) – 5*a(n-2) – 3*a(n-3)           */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset – the behaviour is irrelevant. */
    if (n == 0)
        return (seq_t)0;

    /* Base cases */
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)124;
    if (n == 3) return (seq_t)1300;

    /* Compute iteratively. Only the last three values are needed. */
    seq_t a_minus3 = (seq_t)12;   /* a(1)  */
    seq_t a_minus2 = (seq_t)124;  /* a(2)  */
    seq_t a_minus1 = (seq_t)1300; /* a(3)  */
    seq_t a_cur;

    for (idx_t i = 4; i <= n; ++i) {
        a_cur = (seq_t)11 * a_minus1 - (seq_t)5 * a_minus2 - (seq_t)3 * a_minus3;
        a_minus3 = a_minus2;
        a_minus2 = a_minus1;
        a_minus1 = a_cur;
    }
    return a_cur;
}
