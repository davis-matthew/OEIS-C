/*
 * A040476  Continued fraction for sqrt(499).
 * https://oeis.org/A040476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients of sqrt(499).  The sequence is the
   coefficients in the periodic continued fraction expansion of
   √499.  This implementation computes the period once and returns
   a[n] directly.  a(0) is the integer part ⌊√499⌋. */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static int initialized = 0;
    static seq_t period[128];      /* period coefficients   */
    static size_t period_len = 0;  /* length of the period  */
    static seq_t a0_val;           /* integer part of √499 */

    /* the radicand */
    const idx_t D = 499;

    if (!initialized) {
        /* 1. Compute ⌊√D⌋ using binary search (use idx_t for speed). */
        idx_t low = 0, high = D;
        idx_t a0_idx = 0;
        while (low <= high) {
            idx_t mid = (low + high) >> 1;
            if (mid * mid <= D) {
                a0_idx = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        a0_val = (seq_t)a0_idx;      /* store as seq_t for returning */

        /* 2. Compute the periodic part.  All arithmetic below uses
           idx_t because the values are small (≤ 500). */
        idx_t m = 0;
        idx_t d = 1;
        idx_t a = a0_idx;           /* current a in the algorithm */
        idx_t a_next_idx;

        /* The period ends when the computed a_next equals
           2 * a0_idx (standard property for simple continued
           fractions of square roots). */
        do {
            m = d * a - m;
            d = (D - m * m) / d;
            a_next_idx = (a0_idx + m) / d;
            period[period_len++] = (seq_t)a_next_idx;
            a = a_next_idx;
        } while (a_next_idx != 2 * a0_idx);
        initialized = 1;
    }

    if (n == 0)
        return a0_val;

    /* 3. Return the appropriate periodic coefficient. */
    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
