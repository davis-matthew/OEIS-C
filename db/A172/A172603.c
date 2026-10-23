/*
 * A172603  a(n) = (7n)!/(7!^n).
 * https://oeis.org/A172603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7n)! / (7!^n) offset 0
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute OEIS sequence A172603: a(n) = (7n)!/(7!^n)
 * This implementation uses the recurrence
 *    a(0) = 1,
 *    a(n) = a(n-1) * (∏_{k=0}^{6} (7n-6+k)) / 7!
 * The product of any 7 consecutive integers is divisible by 7!,
 * so the division yields an integer at every step. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;                       /* a(0)=1, a(1)=1 */

    seq_t res = 1;
    const seq_t fact7 = 5040;            /* 7! */

    for (idx_t i = 2; i <= n; ++i) {
        idx_t start = 7 * (i - 1) + 1;   /* first factor of the block */
        seq_t prod = 1;
        for (idx_t k = 0; k < 7; ++k) {
            prod *= (seq_t)(start + k);
        }
        prod /= fact7;                  /* exact division */
        res *= prod;
    }
    return res;
}
