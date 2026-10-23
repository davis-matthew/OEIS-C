/*
 * A040452  Continued fraction for sqrt(474).
 * https://oeis.org/A040452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   periodic continued fraction for sqrt(474)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* static values describing the period of the continued fraction of sqrt(474) */
    static int initialized = 0;
    static seq_t period[64];
    static size_t period_len = 0;

    if (!initialized) {
        /* compute the period the first time this function is called */
        const idx_t N = 474;
        const idx_t a0 = 21;               /* floor(sqrt(474)) */
        idx_t m = 0;
        idx_t d = 1;
        idx_t a_val = a0;
        size_t pos = 0;
        const idx_t twice_a0 = 2 * a0;

        /* generate terms up to and including the final term 2*a0 */
        do {
            m = d * a_val - m;
            d = (N - m * m) / d;
            a_val = (a0 + m) / d;
            period[pos++] = (seq_t)a_val;
        } while (a_val != twice_a0);

        period_len = pos;   /* should be 14 for sqrt(474) */
        initialized = 1;
    }

    if (n == 0u)
        return (seq_t)21u;   /* a(0) = 21 */

    /* a(n) for n >= 1 repeats the period starting with a(1) */
    size_t index = (size_t)((n - 1u) % (idx_t)period_len);
    return period[index];
}
