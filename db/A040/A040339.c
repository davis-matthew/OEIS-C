/*
 * A040339  Continued fraction for sqrt(358).
 * https://oeis.org/A040339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = a(n-26); a(0) = 18; period = [1, 11, 1, 1, 1, 3, 1, 1, 4,
 *            1, 5, 2, 18, 2, 5, 1, 4, 1, 1, 3, 1, 1, 1, 11, 1, 36]
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continues fraction coefficients for sqrt(358).  Uses only integer
   arithmetic and works for all supported seq_t and idx_t types. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Period of the continued fraction – 26 terms. */
    static seq_t period[26];
    static bool init = false;

    if (!init) {
        /* Constants for sqrt(358). */
        const idx_t D = 358;      /* non-square D */
        const idx_t a0 = 18;     /* floor(sqrt(358)) */
        /* Variables following the continued‑fraction algorithm. */
        idx_t m = 0;             /* m0 */
        idx_t d = 1;             /* d0 */
        idx_t a_prev = a0;       /* a0 */

        for (int i = 1; i <= 26; ++i) {
            /* Compute next m and d using the previous a. */
            m = a_prev * d - m;
            d = (D - m * m) / d;            /* exact division */
            /* Compute the next a (the coefficient). */
            idx_t ai = (a0 + m) / d;
            period[i - 1] = (seq_t)ai;
            a_prev = ai;                     /* prepare for next step */
        }
        init = true;
    }

    if (n == 0) {
        return (seq_t)18;                     /* a0 */
    }
    /* Periodic part: a(n) = period[(n-1) mod 26] for n > 0. */
    return period[(n - 1) % 26];
}
