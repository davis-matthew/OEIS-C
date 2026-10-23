/*
 * A105845  Numbers n such that n^2 = 41*m^2 + 41*m + 1.
 * https://oeis.org/A105845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=4511, a(3)=4098*a(1)+a(2)=8609,
 *            a(4)=4098*a(2)+a(1)=18486079, a(n)=4098*a(n-2)-a(n-4) (for n>=5)
 *            This follows the recurrence given in the OEIS entry
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4511;
    if (n == 3) return (seq_t)8609;
    if (n == 4) return (seq_t)18486079;

    /* keep last four values: a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a_nm4 = (seq_t)1;               /* a(1) */
    seq_t a_nm3 = (seq_t)4511;            /* a(2) */
    seq_t a_nm2 = (seq_t)8609;            /* a(3) */
    seq_t a_nm1 = (seq_t)18486079;        /* a(4) */

    const seq_t coeff = (seq_t)4098;

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = coeff * a_nm2 - a_nm4;      /* a(i) = 4098*a(i-2) - a(i-4) */
        /* shift window by one */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }
    return a_nm1;
}
