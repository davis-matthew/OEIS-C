/*
 * A069865  a(n) = Sum_{k = 0..n} C(n,k)^6.
 * https://oeis.org/A069865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0}^n C(n,k)^6 (OEIS A069865)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uintptr_t etc if needed */

/* Compute binomial nCk iteratively and accumulate the 6th power. */
seq_t a(idx_t n)
{
    seq_t sum = 0;          /* Accumulator for the sum */
    seq_t C   = 1;          /* C(n,0) = 1 */

    for (idx_t k = 0; k <= n; ++k) {
        /* Compute C(n,k)^6 using 3 multiplications: C^2, C^3, C^6 = (C^3)^2 */
        seq_t C2 = C * C;          /* C^2 */
        seq_t C3 = C2 * C;         /* C^3 */
        seq_t C6 = C3 * C3;        /* C^6 */

        sum += C6;                 /* Add to total */

        /* Prepare next binomial coefficient for k+1 */
        if (k < n) {
            C = (C * (seq_t)(n - k)) / (seq_t)(k + 1);
        }
    }

    return sum;
}
