/*
 * A094829  Number of (s(0), s(1), ..., s(2n+1)) such that 0 < s(i) < 9 and
 *          |s(i) - s(i-1)| = 1 for i = 1,2,...,2n+1, s(0) = 1, s(2n+1) = 6.
 * https://oeis.org/A094829
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A094829: a(n) = 6*a(n-1) - 9*a(n-2) + a(n-3)
   (offset 2, so a(2)=1, a(3)=6, a(4)=27) */

/* No external state is required; each call computes the value
   from the recurrence using a constant amount of memory. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* undefined for n < 2 */
    if (n == 2) return (seq_t)1;          /* a(2) */
    if (n == 3) return (seq_t)6;          /* a(3) */
    if (n == 4) return (seq_t)27;         /* a(4) */

    /* Initialize the first three known terms */
    seq_t a_nm3 = (seq_t)1;   /* a(n-3) for n=5 */
    seq_t a_nm2 = (seq_t)6;   /* a(n-2) for n=5 */
    seq_t a_nm1 = (seq_t)27;  /* a(n-1) for n=5 */
    seq_t result = a_nm1;     /* will hold a(i) */

    for (idx_t i = 5; i <= n; ++i) {
        result = a_nm1 * (seq_t)6 - a_nm2 * (seq_t)9 + a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = result;
    }
    return result;
}
