/*
 * A054128  T(n,2), array T as in A054126.
 * https://oeis.org/A054128
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) - 14*a(n-2) + 16*a(n-3) - 9*a(n-4) + 2*a(n-5)
 * for n ≥ 8, with initial values
 * a(2)=2, a(3)=7, a(4)=24, a(5)=68, a(6)=171, a(7)=398
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset and small indices directly */
    if (n < 2) return 0;            /* undefined, return 0 */
    if (n <= 7) {
        static const seq_t init[6] = {2, 7, 24, 68, 171, 398};
        return init[n - 2];
    }

    /* Initial values for the recurrence:
     * a3 = a(n-5), a4 = a(n-4), a5 = a(n-3), a6 = a(n-2), a7 = a(n-1)  */
    seq_t a3 = 7;   /* a(3) */
    seq_t a4 = 24;  /* a(4) */
    seq_t a5 = 68;  /* a(5) */
    seq_t a6 = 171; /* a(6) */
    seq_t a7 = 398; /* a(7) */

    for (idx_t k = 8; k <= n; ++k) {
        /* Compute a(k) = 6*a(k-1) - 14*a(k-2) + 16*a(k-3) - 9*a(k-4) + 2*a(k-5) */
        seq_t pos_sum = 6 * a7 + 16 * a5 + 2 * a3;   /* positive terms */
        seq_t neg_sum = 14 * a6 + 9 * a4;            /* negative terms */
        seq_t a_next = pos_sum - neg_sum;            /* pos_sum > neg_sum for all n ≥ 8 */

        /* Shift the window for the next iteration */
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = a7;
        a7 = a_next;
    }

    return a7;   /* a(n) after the loop */
}
