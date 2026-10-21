/*
 * A059506  Transform of A059502 applied to sequence 3,4,5,...
 * https://oeis.org/A059506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence: a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3) - a(n-4)
 * Initial values: a(1)=3, a(2)=7, a(3)=19, a(4)=53
 * (Offset 1)
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset is 1, but guard against n==0 */

    /* Handle the first four terms directly */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)19;
    if (n == 4) return (seq_t)53;

    /* Iteratively compute the recurrence from 5 up to n.
       We keep only the last four computed values: a1=a(n-4), a2=a(n-3),
       a3=a(n-2), a4=a(n-1).  For each step we compute a_n. */
    seq_t a1 = (seq_t)3;   /* a(1)   */
    seq_t a2 = (seq_t)7;   /* a(2)   */
    seq_t a3 = (seq_t)19;  /* a(3)   */
    seq_t a4 = (seq_t)53;  /* a(4)   */
    seq_t ai = a4;

    for (idx_t i = 5; i <= n; ++i) {
        ai = 6 * a4 - 11 * a3 + 6 * a2 - a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }
    return ai;
}
