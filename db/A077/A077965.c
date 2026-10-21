/*
 * A077965  Expansion of 1/(1+2*x^2-x^3).
 * https://oeis.org/A077965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=-2, a(n) = -2*a(n-2) + a(n-3) for n ≥ 3
 * Range:     i8 n <= 12, i16 n <= 28, i32 n <= 54, i64 n <= 110,
 *            i128 n <= 223, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the three base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)-2;

    /* Store the last three values of the sequence.
     * v[0] == a(k-3), v[1] == a(k-2), v[2] == a(k-1) as we iterate.  */
    seq_t v[3];
    v[0] = (seq_t)1;   /* a(0) */
    v[1] = (seq_t)0;   /* a(1) */
    v[2] = (seq_t)-2;  /* a(2) */

    /* Compute up to a(n).  Each step only needs the previous three terms. */
    for (idx_t i = 3; i <= n; ++i)
    {
        /* Next term depends on a(i-2) and a(i-3). These are stored in
         * v[(i-2)%3] and v[(i-3)%3] respectively.  */
        seq_t next = -((seq_t)2 * v[(i - 2) % 3]) + v[(i - 3) % 3];
        v[i % 3] = next;
    }

    return v[n % 3];
}
