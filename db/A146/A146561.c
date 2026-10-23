/*
 * A146561  Numbers m with the property that shifting the rightmost digit of m
 *          to the left end multiplies the number by 3.
 * https://oeis.org/A146561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 7, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A146561: a(n) = m * (10^(28*k) - 1) / 29
 * where k = (n + 6) / 7  (floor division)
 *     m = (n % 7) + 2   for n % 7 != 0, otherwise m = 9
 * This formula follows the OEIS entry:
 *   a(7*k-6) = 3*(10^(28*k)-1)/29
 *   a(7*k-5) = 4*(10^(28*k)-1)/29
 *   ……
 *   a(7*k-1) = 8*(10^(28*k)-1)/29
 *   a(7*k)   = 9*(10^(28*k)-1)/29
 *
 * The function uses the utility pow10(idx_t) to obtain 10^e as a seq_t.
 * All arithmetic is performed in seq_t to support arbitrary‑precision
 * builds. */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset; behaviour is irrelevant. */
    if (n == 0) return 0;

    /* Compute k and the multiplier m following the pattern described. */
    idx_t k   = (n + 6) / 7;           /* floor((n+6)/7) */
    unsigned rem = n % 7;
    unsigned mult = rem ? (rem + 2) : 9;   /* 3 to 9 according to n mod 7 */

    /* Exponent for the power of ten: 28 * k. */
    idx_t exp = 28 * k;

    /* Compute 10^exp, subtract 1, divide by 29, then multiply by mult. */
    seq_t pow10e = pow10(exp);         /* 10^(28*k) */
    seq_t val    = (pow10e - 1) / 29;  /* (10^(28*k) - 1) / 29 */
    val *= mult;                       /* multiply by the appropriate mult */

    return val;
}
