/*
 * A010075  a(n) = sum of base-8 digits of a(n-1) + sum of base-8 digits of
 *          a(n-2).
 * https://oeis.org/A010075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A010075
   Formula: a(0)=0, a(1)=1, a(2)=1;
   a(n)=sum of base‑8 digits of a(n‑1)+sum of base‑8 digits of a(n‑2);
   From n=3 the sequence is periodic with period 16:
   2,3,5,8,6,7,13,13,12,11,9,6,8,7,8,8 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;

    /* Periodic part */
    static seq_t period[16] = {
        (seq_t)2, (seq_t)3, (seq_t)5, (seq_t)8,
        (seq_t)6, (seq_t)7, (seq_t)13, (seq_t)13,
        (seq_t)12, (seq_t)11, (seq_t)9, (seq_t)6,
        (seq_t)8, (seq_t)7, (seq_t)8, (seq_t)8
    };

    idx_t idx = (n - 3) & 15;            /* n ≥ 3, mask to 0‑15 */
    return period[(size_t)idx];
}
