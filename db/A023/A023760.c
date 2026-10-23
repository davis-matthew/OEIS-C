/*
 * A023760  Nialpdromes: digits in base 4 are in nonincreasing order.
 * https://oeis.org/A023760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth nonnegative integer whose base‑4 representation *
 *            has digits in nonincreasing order (Nialpdromes)
 * Range:     u8 n <= 66, u16 n <= 487, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)              /* special case: a(1) = 0 */
        return 0;

    /* index among positive terms (zero‑based) */
    idx_t pos = n - 2;       /* a(2) has pos = 0 */

    /* find the length of the base‑4 representation */
    idx_t prev = 0;
    idx_t k;
    for (k = 1; ; ++k) {
        /* count_k = C(k+3, 3) – 1 : number of positive nialpdromes of length k */
        idx_t count_k = ((idx_t)(k + 3) * (k + 2) * (k + 1) / 6) - 1;
        if (pos < prev + count_k) {
            pos -= prev;   /* rank within this length */
            break;
        }
        prev += count_k;
    }

    /* construct the base‑4 digits lexicographically */
    uint8_t digits[65];      /* enough for the expected lengths (k <= ~60) */
    idx_t r = pos;
    idx_t ub = 3;            /* upper bound for the next digit (nonincreasing) */

    for (idx_t i = 0; i < k; ++i) {
        idx_t remLen = k - i - 1;     /* digits remaining after this one */
        uint8_t start = (i == 0) ? 1 : 0; /* first digit cannot be 0 */

        for (uint8_t d = start; d <= ub; ++d) {
            idx_t count;
            if (remLen == 0) {       /* last position */
                count = 1;
            } else {
                if (d == 0) {
                    count = 1;      /* all remaining digits must be 0 */
                } else if (d == 1) {
                    count = remLen + 1;                /* C(remLen+1,1) */
                } else if (d == 2) {
                    count = (remLen + 2) * (remLen + 1) / 2;   /* C(remLen+2,2) */
                } else { /* d == 3 */
                    count = (remLen + 3) * (remLen + 2) * (remLen + 1) / 6; /* C(remLen+3,3) */
                }
            }

            if (r >= count) {
                r -= count;            /* skip whole block */
            } else {
                digits[i] = d;         /* this digit is fixed */
                ub = d;                /* next digits can't exceed this */
                break;
            }
        }
    }

    /* convert the digit array to a decimal (base-10) value */
    seq_t value = 0;
    for (idx_t i = 0; i < k; ++i) {
        value = value * 4 + digits[i];
    }

    return value;
}
