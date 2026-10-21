/*
 * A052980  Expansion of (1 - x)/(1 - 2*x - x^3).
 * https://oeis.org/A052980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 57, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=2; for n≥3, a(n)=2*a(n-1)+a(n-3) */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t a_nm3 = (seq_t)1;   /* a(n-3)   */
    seq_t a_nm2 = (seq_t)1;   /* a(n-2)   */
    seq_t a_nm1 = (seq_t)2;   /* a(n-1)   */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t a_n = (seq_t)2 * a_nm1 + a_nm3;  /* a(n)=2*a(n-1)+a(n-3) */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;   /* a(n) */
}
