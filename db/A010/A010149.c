/*
 * A010149  Continued fraction for sqrt(70).
 * https://oeis.org/A010149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(n) is the nth partial quotient in the continued‑fraction
 *            expansion of √70. a(0) = ⌊√70⌋ = 8 and the subsequent terms
 *            repeat with period 6: 2, 1, 2, 1, 2, 16
 * Range:     u8 n <= 8397, u16 n <= 8397, u32 n <= 8397, u64 n <= 8397,
 *            u128 n <= 8397, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t a0_val;          /* a(0) */
    static seq_t period[6];      /* periodic part */

    if (!initialized) {
        /* compute the first term */
        a0_val = (seq_t)8; /* floor(sqrt(70)) */

        /* generate the repeating part of the continued fraction */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_cur = a0_val;
        size_t pos = 0;

        while (true) {
            /* next term of the continued‑fraction algorithm */
            m = d * a_cur - m;
            d = ((seq_t)70 - m * m) / d;
            a_cur = (a0_val + m) / d;

            period[pos++] = a_cur;

            /* period ends when a_cur == 2*a0_val */
            if (a_cur == a0_val * 2)
                break;
        }
        (void)pos;      /* suppress unused warning */
        initialized = true;
    }

    if (n == 0)
        return a0_val;

    size_t idx = (size_t)((n - 1ULL) % 6ULL);
    return period[idx];
}
