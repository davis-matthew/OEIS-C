/*
 * A056159  a(n) = floor(10^(n-1)/n).
 * https://oeis.org/A056159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊10^(n-1) / n⌋
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) = 1 */
    if (n == 1) return (seq_t)1;

    /* Perform long division of 1 followed by (n‑1) zeros by n
       while building the result digit by digit.  All intermediate
       numeric work fits in a 32‑bit integer. */
    uint32_t remainder = 1;          /* current dividend fragment (0 ≤ remainder < n) */
    seq_t result = 0;                /* accumulated quotient */
    for (idx_t i = 1; i < n; ++i) {
        uint32_t cur = remainder * 10;          /* bring down next decimal zero */
        uint32_t digit = cur / (uint32_t)n;     /* next quotient digit 0‑9 */
        remainder = cur % (uint32_t)n;          /* new remainder for next step */
        result = result * 10 + digit;           /* append digit to result */
    }
    return result;
}
