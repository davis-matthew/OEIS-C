/*
 * A032069  Number of reversible strings with n labeled beads of 2 colors, no
 *          palindromes of more than 1 bead.
 * https://oeis.org/A032069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * A032085(n) with A032085(1)=2, A032085(2)=1,
 *            A032085(3)=2 and for n≥4 A032085(n) = 2*A032085(n-1) if n is
 *            odd, A032085(n) = 2*A032085(n-1) + 2^(n/2-1) if n is even. This
 *            recurrence reproduces the OEIS sequence A032069
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* offset is 1 */

    seq_t fact = 1;                      /* i!  */
    seq_t A_prev = 0, A_curr = 0;          /* A032085 values */
    seq_t pow2_even = 1;                  /* holds 2^(i/2-1) for even i   */

    for (idx_t i = 1; i <= n; ++i) {
        /* compute A032085(i) */
        if (i == 1) {
            A_curr = (seq_t)2;
        } else if (i == 2) {
            A_curr = (seq_t)1;
        } else if (i == 3) {
            A_curr = (seq_t)2;
        } else {
            if (i & 1) {                  /* odd i */
                A_curr = A_prev * (seq_t)2;
            } else {                      /* even i >= 4 */
                if (i > 2)
                    pow2_even = pow2_even * (seq_t)2; /* 2^(i/2-1) */
                A_curr = A_prev * (seq_t)2 + pow2_even;
            }
        }

        /* update factorial i! */
        fact = fact * (seq_t)i;

        /* result requested? */
        if (i == n)
            return fact * A_curr;

        /* shift A for next iteration */
        A_prev = A_curr;
    }
    return (seq_t)0; /* unreachable */
}
