/*
 * A021718  Decimal expansion of 1/714.
 * https://oeis.org/A021718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 714) mod 10 (nth digit of the decimal
 *            expansion of 1/714)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The remainder of 1/714 always stays less than 714, so we
       can compute each decimal digit by a simple division */
    const uint64_t denom = 714;
    uint64_t r = 1;      /* start with the numerator of the fraction 1/714 */
    seq_t digit = 0;
    idx_t i = 0;

    /* Compute digits one by one until we reach the requested position.
       The loop works even for large n because the remainder stays small. */
    while (1) {
        r *= 10;                      /* bring down the next zero */
        digit = (seq_t)(r / denom);    /* current decimal digit */
        r %= denom;                   /* new remainder for the next step */
        if (i == n)
            break;
        ++i;
    }
    return digit;
}
