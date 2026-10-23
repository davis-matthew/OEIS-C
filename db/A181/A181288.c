/*
 * A181288  Numbers of the form i*6^j-1 (i=1..5, j >= 0).
 * https://oeis.org/A181288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (( ( (n-1)/5 + 1 ) * 6^floor((n-1)/5) )) - 1 The sequence
 *            is obtained by listing the numbers of the form i*6^j-1 (i=1..5,
 *            j>=0) in non‑decreasing order. Because 6^j grows strictly faster
 *            than the factor i, all numbers with a smaller exponent j are
 *            smaller than those with any larger exponen...
 * Range:     u8 n <= 16, u16 n <= 31, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast exponentiation by squaring, using seq_t as the result type.
   This works for arbitrary‑precision seq_t and for 64‑bit values. */
static seq_t pow6(idx_t exp) {
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)6;
    while (exp) {
        if (exp & (idx_t)1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t j = (n - 1) / 5;                     /* exponent of 6   */
    seq_t i = (seq_t)((n - 1) % 5) + 1;        /* multiplier (1..5) */

    seq_t pow = pow6(j);                       /* compute 6^j */
    seq_t val = i * pow;                       /* i*6^j          */
    val -= (seq_t)1;                           /* subtract 1      */
    return val;
}
