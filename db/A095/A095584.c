/*
 * A095584  a(n) = 96 written in base n.
 * https://oeis.org/A095584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 96 written in base n */
/* Computes the base‑n representation of 96 and concatenates the
   decimal digits of each base‑n digit into a single integer. */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Unary case (base 1).  The representation is 96 consecutive
       digits '1'. */
    if (n == 1) {
        seq_t r = 0;
        for (idx_t i = 0; i < 96; ++i)
            r = r * 10 + 1;
        return r;
    }

    /* Compute the base‑n digits of 96, least‑significant first. */
    seq_t temp = (seq_t)96;
    seq_t digits[16];
    int dcnt = 0;
    while (temp > 0) {
        digits[dcnt++] = temp % (seq_t)n;
        temp /= (seq_t)n;
    }

    /* Build the concatenated decimal number from most‑significant
       digit to least‑significant. */
    seq_t result = 0;
    for (int i = dcnt - 1; i >= 0; --i) {
        seq_t digit = digits[i];

        /* Determine how many decimal digits the base‑n digit
           occupies. */
        int dlen = 1;
        seq_t t = digit;
        while (t >= 10) {
            t /= 10;
            ++dlen;
        }

        result = result * pow10((idx_t)dlen) + digit;
    }
    return result;
}
