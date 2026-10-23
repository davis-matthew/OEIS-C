/*
 * A134795  Concatenation of first n positive oblong numbers.
 * https://oeis.org/A134795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = decimal concatenation of the first n oblong numbers
 *            (oblong(k) = k*(k+1), 1 ≤ k ≤ n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"     /* pow10(idx_t e) returns seq_t */
#include "../utils/digit_len.h" /* digit_len(seq_t x) returns int */

seq_t a(idx_t n)
{
    /* Guard against n < 1: the sequence is not defined there.
       Return 0 as a harmless default. */
    if (n == 0)
        return (seq_t)0;

    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t oblong = (seq_t)k * ((seq_t)k + 1);          /* k*(k+1)   */
        int d = digit_len(oblong);                       /* digit count */
        result = result * pow10((idx_t)d) + oblong;       /* append */
    }
    return result;
}
