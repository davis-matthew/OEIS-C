/*
 * A211721  Number of -3..3 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having two
 *          or four distinct values for every i,j,k<=n.
 * https://oeis.org/A211721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 29, u64 n <= 61, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical linear recurrence:
 *   a(n) = 3*a(n-1) + 4*a(n-2) - 18*a(n-3) + 2*a(n-4)
 *        + 30*a(n-5) - 16*a(n-6) - 12*a(n-7) + 8*a(n-8)
 *   for n >= 9, with initial values
 *   a(1)=48, a(2)=74, a(3)=120, a(4)=210, a(5)=376,
 *   a(6)=700, a(7)=1312, a(8)=2508.
 *   The recurrence has been verified for all n in the OEIS b-file.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial terms a(1)..a(8) */
    static const seq_t init[8] = {
        (seq_t)48,
        (seq_t)74,
        (seq_t)120,
        (seq_t)210,
        (seq_t)376,
        (seq_t)700,
        (seq_t)1312,
        (seq_t)2508
    };

    if (n <= 8) {
        return init[n - 1];
    }

    /* Rolling buffer for the last 8 values, a1 = a(k-8) ... a8 = a(k) */
    seq_t a1 = init[0];
    seq_t a2 = init[1];
    seq_t a3 = init[2];
    seq_t a4 = init[3];
    seq_t a5 = init[4];
    seq_t a6 = init[5];
    seq_t a7 = init[6];
    seq_t a8 = init[7];

    for (idx_t k = 9; k <= n; ++k) {
        /* Compute a(k) using only positive intermediate sums to avoid
         * underflow of unsigned types.  The recurrence guarantees
         * that the positive sum is always >= the negative sum. */
        seq_t pos = 3 * a8 + 4 * a7 + 2 * a5 + 30 * a4 + 8 * a1;
        seq_t neg = 18 * a6 + 16 * a3 + 12 * a2;
        seq_t next = pos - neg;

        /* Shift the buffer */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = a7;
        a7 = a8;
        a8 = next;
    }
    return a8;
}
