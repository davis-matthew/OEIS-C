/*
 * A040774  Continued fraction for sqrt(803).
 * https://oeis.org/A040774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(803):
   a(0) = 28 (integer part of sqrt(803))
   for n ≥ 1, a(n) repeats with period [2, 1, 27, 1, 2, 56]   */
/* The algorithm computes the period only once and then behaves in O(1). */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static data to avoid recomputing the period. */
    static bool init = false;
    static uint64_t a0_uint;            /* integer part of sqrt(803)           */
    static uint64_t period[10];         /* holds the 6 periodic terms         */
    static size_t period_len = 0;       /* length of the period                */

    if (!init) {
        const uint64_t D = 803ULL;

        /* Compute a0 = floor(sqrt(D)) explicitly – 28 for D=803. */
        a0_uint = 28U;   /* we could use a generic integer sqrt, but 28 is trivial */

        /* Compute the periodic part of the continued fraction. */
        uint64_t m = 0, d = 1, a_val = a0_uint;
        do {
            m = d * a_val - m;
            uint64_t mm = m * m;
            d = (D - mm) / d;
            a_val = (a0_uint + m) / d;
            period[period_len++] = a_val;
        } while (a_val != 2 * a0_uint);
        init = true;
    }

    if (n == 0)
        return (seq_t)a0_uint;

    /* For n ≥ 1: return the (n‑1)-th element of the period. */
    size_t idx = (size_t)((n - 1) % period_len);
    return (seq_t)period[idx];
}
