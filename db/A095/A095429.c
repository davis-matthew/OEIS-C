/*
 * A095429  a(n) = 14 written in base n.
 * https://oeis.org/A095429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal representation of 14 written in base n
 * Range:     u8 none, u16 none, u32 none, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* external linkage as required */
seq_t a(idx_t n)
{
    /* Base‑1 (unary) representation – 14 consecutive 1’s */
    if (n == 1) {
        seq_t r = 0;
        for (int i = 0; i < 14; ++i) {
            r = r * 10 + 1;
        }
        return r;
    }

    unsigned long long base = (unsigned long long)n;
    unsigned long long rem   = 14;          /* the number to convert          */
    unsigned char digits[64];              /* most digits needed are < 64    */
    int len = 0;

    /* Special case: 0 – not needed for this sequence but keeps loop logic */
    if (rem == 0) {
        digits[len++] = 0;
    } else {
        /* Convert to base 'base', least significant digit first */
        while (rem > 0) {
            digits[len++] = (unsigned char)(rem % base);
            rem /= base;
        }
    }

    /* Build the decimal number consisting of the base‑digit string */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        result = result * 10 + digits[i];
    }

    return result;
}
