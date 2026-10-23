/*
 * A191111  Describe 10^n. Also called the "Say What You See" or "Look and
 *          Say" sequence LS(10^n).
 * https://oeis.org/A191111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 11, for n = 0; for n > 0, a(n) = 11 · 10^(d(n)+1) + n ·
 *            10 where d(n) is the number of decimal digits of n
 * Range:     u8 n <= 0, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

/* Count decimal digits of an idx_t value (x=0 gives 1). */
static idx_t digit_count(idx_t x)
{
    idx_t d = 0;
    do { d++; x = x / 10; } while (x);
    return d;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)11;

    idx_t d = digit_count(n);
    seq_t pow = pow10((idx_t)(d + 1));           /* 10^(d+1) */
    seq_t res = (seq_t)11 * pow + (seq_t)n * (seq_t)10;
    return res;
}
