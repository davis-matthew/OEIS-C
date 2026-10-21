/*
 * A054136  a(n) = T(n,2), array T as in A054134.
 * https://oeis.org/A054136
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 9*a(n-2) + 7*a(n-3) - 2*a(n-4) + p(n) * where
 *            p(n) = 2,-4,6,-4,1 for n = 2,3,4,5,6 respectively, and p(n)=0
 *            for n>6. * The sequence starts at offset 2: a(2)=2
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A054136: compute T(n,2) from the triangle of trinomial coefficients A054134. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle offset and base cases */
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)18;
    if (n == 5) return (seq_t)46;

    /* rolling window for a(n-1) .. a(n-4) */
    seq_t a_n4 = (seq_t)2;   /* a(2) */
    seq_t a_n3 = (seq_t)6;   /* a(3) */
    seq_t a_n2 = (seq_t)18;  /* a(4) */
    seq_t a_n1 = (seq_t)46;  /* a(5) */

    seq_t cur = 0;
    for (idx_t i = 6; i <= n; ++i) {
        seq_t p = 0;
        if (i == 6) p = (seq_t)1;          /* p(6) = 1 */
        /* recurrence */
        cur = (seq_t)5 * a_n1 - (seq_t)9 * a_n2 + (seq_t)7 * a_n3 - (seq_t)2 * a_n4 + p;

        /* shift window for next iteration */
        a_n4 = a_n3;
        a_n3 = a_n2;
        a_n2 = a_n1;
        a_n1 = cur;
    }
    return a_n1;
}
