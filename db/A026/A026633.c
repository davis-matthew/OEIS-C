/*
 * A026633  a(n) = Sum_{k=0..n} A026626(n, k).
 * https://oeis.org/A026633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} A026626(n, k). Closed form for n>=2: a(n) =
 *            (17*2^(n-2) + (-1)^n)/3 - 1 Recurrence (valid for n>=4): a(n) =
 *            a(n-1) + 2*a(n-2) + 2 Initial values: a(0)=1, a(1)=2, a(2)=5,
 *            a(3)=10
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* framework requirement */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)10;

    seq_t a_nm2 = (seq_t)5;   /* a(i-2) for i = 4 */
    seq_t a_nm1 = (seq_t)10;  /* a(i-1) */
    seq_t cur   = 0;

    for (idx_t i = 4; i <= n; ++i) {
        /* cur = a(i-1) + 2*a(i-2) + 2 */
        cur = a_nm1 + (a_nm2 + a_nm2) + (seq_t)2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return cur;
}
