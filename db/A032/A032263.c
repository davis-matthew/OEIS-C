/*
 * A032263  Number of ways to partition n labeled elements into 4 pie slices
 *          allowing the pie to be turned over; number of 2-element proper
 *          antichains of an n-element set.
 * https://oeis.org/A032263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 3 * S(n,4) where S(n,k) is the Stirling number of the
 *            second kind. Recurrence used: S(0,0)=1, S(0,k>0)=0;
 *            S(n,k)=k*S(n-1,k)+S(n-1,k-1)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The helper mul_small(seq_t, unsigned) is defined in utils/mul_small.h */
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* S[k] holds S(current_row, k).  We need k = 0..4. */
    seq_t S[5] = {1, 0, 0, 0, 0};
    seq_t T[5];

    for (idx_t i = 1; i <= n; ++i) {
        T[0] = 0;                                  /* S(i,0) = 0 for i>0 */
        for (idx_t k = 1; k <= 4; ++k) {
            /* S(i,k) = k*S(i-1,k) + S(i-1,k-1) */
            T[k] = mul_small(S[k], (unsigned)k) + S[k-1];
        }
        /* copy the new row back into S */
        for (idx_t k = 0; k <= 4; ++k) S[k] = T[k];
    }

    /* a(n) = 3 * S(n,4) */
    return mul_small(S[4], 3);
}
