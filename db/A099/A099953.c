/*
 * A099953  a(n) = A076795(n) - 1.
 * https://oeis.org/A099953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-2)*a(n-1) - (2*n-3)*a(n-2) with a(1)=0, a(2)=1. This
 *            is the recurrence for OEIS A099953 = A076795(n)-1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the base cases. */
    if (n <= 1)
        return (seq_t)0;            /* a(1) = 0 */
    if (n <= 2)
        return (seq_t)1;            /* a(2) = 1 */

    /* Iterate the recurrence from n = 3 up to the requested n. */
    seq_t a_prev = (seq_t)0;        /* a(1) */
    seq_t a_curr = (seq_t)1;        /* a(2) */
    seq_t a_next;

    for (idx_t i = 3; i <= n; ++i) {
        /* Compute (2*i-2) and (2*i-3) as seq_t values.  */
        seq_t mul_a = (seq_t)((2u * (idx_t)i) - 2u);  /* 2*i-2 */
        seq_t mul_b = (seq_t)((2u * (idx_t)i) - 3u);  /* 2*i-3 */

        a_next = mul_a * a_curr - mul_b * a_prev;
        a_prev = a_curr;
        a_curr = a_next;
    }

    return a_curr;
}
