/*
 * A028184  Expansion of 1/((1-5x)(1-7x)(1-9x)(1-10x)).
 * https://oeis.org/A028184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 31*a(n-1) - 353*a(n-2) + 1745*a(n-3) - 3150*a(n-4) with
 *            a(0)=1, a(1)=31, a(2)=608, a(3)=9650
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)31;
    if (n == 2) return (seq_t)608;
    if (n == 3) return (seq_t)9650;

    /* Initialize the recurrence. */
    seq_t a0 = (seq_t)1;      /* a(n-4) */
    seq_t a1 = (seq_t)31;     /* a(n-3) */
    seq_t a2 = (seq_t)608;    /* a(n-2) */
    seq_t a3 = (seq_t)9650;   /* a(n-1) */

    /* Compute up to n by the linear recurrence. */
    for (idx_t i = 4; i <= n; ++i) {
        /* The order of terms ensures the intermediate results stay
           non‑negative for all n >= 4. */
        seq_t next = (seq_t)31 * a3 + (seq_t)1745 * a1
                     - (seq_t)353 * a2 - (seq_t)3150 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
