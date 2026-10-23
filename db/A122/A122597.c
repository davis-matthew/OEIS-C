/*
 * A122597  a(0) = 1, a(1) = 2, s = 1; for n >= 2, if a(n-1) is even and s = 0
 *          then set a(n) = a(n-1)/2 and s = 1; otherwise set a(n) = a(n-1) +
 *          a(n-2) and s = 0.
 * https://oeis.org/A122597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 45, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Recurrence: a(0)=1, a(1)=2; for n>=2:
   if a(n-1) is even and s=0 then a(n)=a(n-1)/2 and s=1;
   otherwise a(n)=a(n-1)+a(n-2) and s=0. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t a_nm2 = (seq_t)1;          /* a(n-2) */
    seq_t a_nm1 = (seq_t)2;          /* a(n-1) */
    bool s = true;                  /* s after a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t a_n;
        if (((a_nm1 & (seq_t)1) == (seq_t)0) && !s) {
            a_n = a_nm1 / (seq_t)2;
            s = true;
        } else {
            a_n = a_nm1 + a_nm2;
            s = false;
        }
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
