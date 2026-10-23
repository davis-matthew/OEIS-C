/*
 * A146885  a(n) = 8*Sum_{k=0..n} 7^k.
 * https://oeis.org/A146885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 8 * Sum_{k=0..n} 7^k Recurrence: a(0)=8, a(1)=64,
 *            a(n)=8*a(n-1)-7*a(n-2) for n>=2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)8;
    if (n == 1) return (seq_t)64;

    seq_t a_nm2 = (seq_t)8;          /* a(0) */
    seq_t a_nm1 = (seq_t)64;         /* a(1) */
    seq_t a_n;                       /* current a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        a_n = a_nm1 * (seq_t)8 - a_nm2 * (seq_t)7;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
