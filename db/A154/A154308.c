/*
 * A154308  a(n) = (n+1)^5*(5+n)!/120.
 * https://oeis.org/A154308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^5 * (n+5)! / 120
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Prime factor counts required in the denominator 120 = 2^3 * 3^1 * 5^1 */
    unsigned int need2 = 3;
    unsigned int need3 = 1;
    unsigned int need5 = 1;

    seq_t result = 1;
    idx_t m = n + 5;          /* upper bound of the factorial */
    seq_t base = (seq_t)(n + 1);

    /* Multiply the factorial terms, cancelling any 2, 3 or 5 factors
       that are still needed for the final division by 120. */
    for (idx_t i = 2; i <= m; ++i) {
        seq_t v = (seq_t)i;

        /* cancel factors of 2 */
        if (need2 && (v & 1) == 0) {
            while ((v & 1) == 0 && need2) {
                v >>= 1;          /* v /= 2; */
                need2--;
            }
        }
        /* cancel factors of 3 */
        if (need3 && v % 3 == 0) {
            while (v % 3 == 0 && need3) {
                v /= 3;
                need3--;
            }
        }
        /* cancel factors of 5 */
        if (need5 && v % 5 == 0) {
            while (v % 5 == 0 && need5) {
                v /= 5;
                need5--;
            }
        }
        result *= v;
    }

    /* Multiply (n+1)^5, again cancelling any remaining factors
       required for division by 120. */
    for (unsigned int k = 0; k < 5; ++k) {
        seq_t v = base;

        if (need2 && (v & 1) == 0) {
            while ((v & 1) == 0 && need2) {
                v >>= 1;
                need2--;
            }
        }
        if (need3 && v % 3 == 0) {
            while (v % 3 == 0 && need3) {
                v /= 3;
                need3--;
            }
        }
        if (need5 && v % 5 == 0) {
            while (v % 5 == 0 && need5) {
                v /= 5;
                need5--;
            }
        }
        result *= v;
    }

    /* If any denominator factors remain (unlikely), divide them out. */
    if (need2 || need3 || need5) {
        seq_t rem = 1;
        while (need2) { rem *= 2; need2--; }
        while (need3) { rem *= 3; need3--; }
        while (need5) { rem *= 5; need5--; }
        result /= rem;
    }

    return result;
}
