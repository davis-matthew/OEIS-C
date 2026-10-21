/*
 * A067894  Write 0, 1, ..., n in binary and add as if they were decimal
 *          numbers.
 * https://oeis.org/A067894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum_{k=0}^{n} interpret_decimal(binary(k))
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 283, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Interpret binary representation of k as a decimal number and sum. */

#include <stdint.h>

/* The function uses only integer arithmetic allowed on seq_t. */
seq_t a(idx_t n)
{
    seq_t result = 0;

    /* Determine the maximum bit position needed */
    idx_t maxBit = 0;
    idx_t temp = n;
    while (temp) {
        maxBit++;
        temp >>= 1;
    }
    /* Handle the trivial case n == 0 */
    if (maxBit == 0)
        return result;    /* result is already 0 */

    /* Precompute powers of 10 up to the maximum bit index */
    seq_t pow10[65];
    pow10[0] = (seq_t)1;
    for (idx_t i = 1; i <= maxBit; ++i) {
        pow10[i] = pow10[i - 1] * (seq_t)10;
    }

    /* Sum decimal interpretations of all binary numbers from 0 to n */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t value = 0;
        idx_t m = k;
        idx_t bitPos = 0;
        while (m) {
            if (m & (idx_t)1) {
                value += pow10[bitPos];
            }
            m >>= 1;
            ++bitPos;
        }
        /* For k == 0 the loop body is skipped; value remains 0 as desired */
        result += value;
    }

    return result;
}
