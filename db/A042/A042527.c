/*
 * A042527  Denominators of continued fraction convergents to sqrt(792).
 * https://oeis.org/A042527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued‑fraction convergents to sqrt(792)
   (OEIS A042527)

   Recurrence:
     a(0) = 1
     a(1) = 7
     for n ≥ 2:
       if n even: a(n) = 56·a(n‑1) + a(n‑2)
       if n odd : a(n) =   7·a(n‑1) + a(n‑2)
*/

#include <stddef.h>   /* for size_t, if needed */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    /* Use two running variables for the last two terms. */
    seq_t a_nm2 = (seq_t)1;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)7;   /* a(n-1) */
    seq_t a_n;                /* a(n) */

    /* Loop from n = 2 up to the requested index. */
    for (idx_t i = 2; i <= n; ++i) {
        if ((i & 1u) == 0u) {            /* even i */
            a_n = (seq_t)56 * a_nm1 + a_nm2;
        } else {                         /* odd i */
            a_n = (seq_t)7  * a_nm1 + a_nm2;
        }
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}
