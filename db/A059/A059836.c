/*
 * A059836  Triangle T(s,t), s >= 1, 1 <= t <= s (see formula line).
 * https://oeis.org/A059836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(s,t) where the triangle T is defined by T(s, t) =
 *            (s-1)^2·(s-2)^2·…·(s-((t-1)/2))^2 if t is odd T(s, t) =
 *            (s-1)^2·…·(s-(t/2-1))^2·(s-t/2) if t is even The index n is
 *            mapped to the row s and column t by s(s+1)/2 ≥ n > (s-1)s/2, t =
 *            n - (s-1)s/2
 * Range:     u8 n <= 19, u16 n <= 35, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row s such that n lies in that row. */
    idx_t s = 1;
    while ((s * (s + 1) / 2) < n)
        ++s;

    /* Compute column t (1‑based). */
    idx_t prev_total = (s - 1) * s / 2;
    idx_t t = n - prev_total;

    seq_t result = 1;
    if (t & 1ULL) {            /* t is odd */
        /* Multiply squares for i = 1 .. t/2 */
        for (idx_t i = 1; i <= t / 2; ++i) {
            seq_t val = (seq_t)(s - i);
            result *= val;
            result *= val;
        }
    } else {                   /* t is even */
        /* Multiply squares for i = 1 .. t/2-1 */
        for (idx_t i = 1; i < t / 2; ++i) {
            seq_t val = (seq_t)(s - i);
            result *= val;
            result *= val;
        }
        /* Final linear factor */
        result *= (seq_t)(s - t / 2);
    }

    return result;
}
