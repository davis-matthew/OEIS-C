/*
 * A037431  Positive numbers having the same set of digits in base 5 and base
 *          8.
 * https://oeis.org/A037431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 447, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037431: a(n) is the nth positive integer whose set of digits
 * in base 5 equals the set of digits in base 8 (digits 0–4 only).
 * The implementation enumerates numbers whose base‑8 representation
 * uses only digits 0–4 by interpreting each base‑5 integer as a
 * base‑8 number.  For each candidate it checks the digit‑set equality
 * and collects the desired term.
 */

#include <stdbool.h>

/* Return the nth term (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                     /* undefined for offset 1 but harmless   */
    }

    seq_t count = 0;                  /* number of terms found                */
    enum { MAX_DIGITS = 64 };         /* sufficient for all needed lengths     */
    uint8_t digits[MAX_DIGITS];       /* digits of the current base‑5 integer */

    for (idx_t i = 1; ; ++i) {        /* iterate over base‑5 numbers           */
        /* Convert i to base‑5 digits and build mask of those digits.
         * The same digits will be used as base‑8 digits.            */
        idx_t tmp = i;
        int   len = 0;
        uint8_t mask8 = 0;
        while (tmp > 0) {
            uint8_t d = (uint8_t)(tmp % 5);
            digits[len++] = d;
            mask8 |= 1U << d;
            tmp /= 5;
        }

        /* Construct the candidate number in base‑8:
         * val = digits[most significant] * 8^(len-1) + ...        */
        seq_t val = 0;
        for (int k = len - 1; k >= 0; --k) {
            val = val * 8 + digits[k];
        }

        /* Compute the set of digits appearing in the base‑5
         * representation of the candidate number.                 */
        seq_t t = val;
        uint8_t mask5 = 0;
        if (t == 0) {
            mask5 |= 1U << 0;
        } else {
            while (t != 0) {
                uint8_t d = (uint8_t)(t % 5);
                mask5 |= 1U << d;
                t /= 5;
            }
        }

        /* If the two masks match, we have a sequence element.    */
        if (mask5 == mask8) {
            ++count;
            if (count == n) {
                return val;
            }
        }
    }
}
