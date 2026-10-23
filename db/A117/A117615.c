/*
 * A117615  a(0)=0, a(1)=1, a(n)=4a(n-1)+2 for n =3,5,7,..., a(n)=4a(n-1) for
 *          n =2,4,6,....
 * https://oeis.org/A117615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(n)=4*a(n-1) for even n ≥ 2, a(n)=4*a(n-1)+2
 *            for odd n ≥ 3 (equivalently, a(n)=3*a(n-1)+4*a(n-2)+2 for n≥3)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;   /* a(2)=4*a(1) = 4 */

    /* iterate from n=3 up to the requested index using the 2‑term recurrence */
    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)4;   /* a(2) */
    seq_t a_cur;

    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = 3*a(i-1) + 4*a(i-2) + 2 */
        a_cur = 3 * prev1 + 4 * prev2 + (seq_t)2;
        prev2 = prev1;
        prev1 = a_cur;
    }
    return prev1;
}
