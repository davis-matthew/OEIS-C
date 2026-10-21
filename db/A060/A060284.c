/*
 * A060284  Periodic part of decimal expansion of 1/n (leading 0's omitted).
 * https://oeis.org/A060284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = periodic part of decimal expansion of 1/n (leading 0's
 *            omitted)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 16, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Compute the periodic part (repetend) of 1/n in decimal,
   with any leading zeros removed. */
seq_t a(idx_t n) {
    /* Strip factors 2 and 5 to test for a terminating decimal */
    idx_t m = n;
    while ((m & 1ULL) == 0ULL) m >>= 1ULL;          /* remove powers of 2 */
    while ((m % 5ULL) == 0ULL) m /= 5ULL;          /* remove powers of 5 */
    if (m == 1ULL) return (seq_t)0;                /* terminating decimal */

    /* Prepare arrays to track seen remainders and digits of the expansion */
    int *seen = (int *)malloc((size_t)n * sizeof(int));
    if (!seen) return (seq_t)0;
    for (idx_t i = 0; i < n; ++i) seen[i] = -1;

    unsigned char *digits = (unsigned char *)malloc((size_t)n * sizeof(unsigned char));
    if (!digits) { free(seen); return (seq_t)0; }

    idx_t pos = 0;
    idx_t rem = 1 % n;                              /* initial remainder */

    /* Long division simulation until a remainder repeats */
    while (seen[rem] == -1) {
        seen[rem] = (int)pos;                       /* store first occurrence */
        rem = rem * 10;
        unsigned char digit = (unsigned char)(rem / n);
        digits[pos] = digit;
        rem = rem % n;
        ++pos;
    }

    idx_t cycle_start = seen[rem];                  /* start of the repetend */

    /* Build the integer from the repetend digits, omitting leading zeros */
    seq_t result = 0;
    bool started = false;
    for (idx_t i = cycle_start; i < pos; ++i) {
        unsigned char d = digits[i];
        if (!started) {
            if (d == 0) continue;                  /* skip leading zeros */
            started = true;
            result = (seq_t)d;
        } else {
            result = result * 10 + d;
        }
    }
    if (!started) result = (seq_t)0;                /* all zeros case */

    free(seen);
    free(digits);
    return result;
}
