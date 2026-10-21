/*
 * A095563  a(n) = 85 written in base 14 - n.
 * https://oeis.org/A095563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal integer formed by writing 85 in base (14 - n)
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    int64_t base = (int64_t)14 - (int64_t)n;

    /* 1‑base is the unary representation – 85 ones. */
    if (base == 1 || base == -1) {
        seq_t res = 0;
        for (idx_t i = 0; i < (idx_t)85; ++i)
            res = res * (seq_t)10 + (seq_t)1;
        return res;
    }

    /* Base zero is not meaningful for a positional system; by convention
       return the value itself.  This branch will never be reached for
       the OEIS data but protects against accidental use. */
    if (base == 0) {
        return (seq_t)85;
    }

    /* General case: base ≥ 2 or base ≤ –2.  We perform the standard
       division algorithm, adjusted so that the remainder is always
       non‑negative and lies in [0, |b|‑1]. */
    int64_t val = 85;          /* number to be represented            */
    int digits[128];           /* place MSD last, LSD first          */
    int len = 0;
    int64_t absb = base > 0 ? base : -base;

    do {
        int64_t rem = val % base;
        if (rem < 0)          /* make remainder non‑negative        */
            rem += absb;
        digits[len++] = (int)rem;
        val = (val - rem) / base;
    } while (val != 0);

    /* Convert the digit array to a decimal integer.  Each digit is at
       most 9 in the range 0 ≤ b ≤ 14 or |b| ≥ 2, so it fits in one
       decimal position. */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i)
        result = result * (seq_t)10 + (seq_t)digits[i];

    return result;
}
