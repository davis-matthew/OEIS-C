/*
 * A104013  First digit-cycle of binary expansion of 1/n. Any initial 0's are
 *          to be placed at end of cycle.
 * https://oeis.org/A104013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the integer formed by the repeating binary digits of
 *            1/n, with any leading zeros moved to the end
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 12, u64 n <= 28, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute a(n) for OEIS sequence A104013 */
seq_t a(idx_t n)
{
    /* n must be >= 1; if less, return 0 safely */
    if (n == 0) return 0;

    /* Remove all factors of 2 from n to obtain the odd denominator */
    idx_t denom = n;
    while ((denom & 1ULL) == 0ULL) {
        denom >>= 1;
    }

    /* If denom == 1 then the binary expansion terminates → cycle is 0 */
    if (denom == 1ULL) {
        return 0;
    }

    /* Allocate arrays for visited remainders and for the binary digits of the cycle */
    idx_t *pos  = (idx_t *)malloc(denom * sizeof(idx_t));
    uint8_t *bits = (uint8_t *)malloc(denom * sizeof(uint8_t));
    if (!pos || !bits) {
        /* OOM guard – return 0 (value will be ignored by the caller) */
        free(pos);
        free(bits);
        return 0;
    }

    /* mark all remainders as not seen */
    for (idx_t i = 0; i < denom; ++i) {
        pos[i] = (idx_t)-1;
    }

    /* Long division algorithm to extract repeating binary digits of 1/denom */
    idx_t rem = 1 % denom;          /* initial remainder (rem=1 as denom>1) */
    idx_t i = 0;                    /* current index in the cycle */

    while (rem != 0 && pos[rem] == (idx_t)-1) {
        pos[rem] = i;               /* remember first occurrence of this remainder */
        rem <<= 1;                 /* multiply remainder by 2 (next binary digit) */
        bits[i] = (uint8_t)(rem / denom);  /* 0 or 1 */
        rem %= denom;              /* new remainder for next step */
        ++i;
    }

    free(pos);

    /* If the cycle consists only of zeros, return 0 */
    if (i == 0) {
        free(bits);
        return 0;
    }

    /* Find first '1' in the cycle to rotate the string appropriately */
    idx_t first_one = i;            /* unchanged if all zeros (handled above) */
    for (idx_t j = 0; j < i; ++j) {
        if (bits[j] == 1) {
            first_one = j;
            break;
        }
    }
    if (first_one == i) {           /* all zeros – shouldn't happen, but guard */
        free(bits);
        return 0;
    }

    /* Convert the rotated binary digit string to a decimal integer */
    seq_t result = 0;
    for (idx_t j = first_one; j < i; ++j) {
        result = result * 10 + bits[j];
    }
    for (idx_t j = 0; j < first_one; ++j) {
        result = result * 10 + bits[j];
    }

    free(bits);
    return result;
}
