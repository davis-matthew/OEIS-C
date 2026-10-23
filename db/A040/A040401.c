/*
 * A040401  Continued fraction for sqrt(422).
 * https://oeis.org/A040401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Generates the regular continued‑fraction terms of √422.
   a(0)=⌊√422⌋, a(1…) are the periodic part of the continued fraction.
   The period is computed using the standard quadratic‑surd recurrence
   and memoised statically for fast repeated access. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    static seq_t period[64];
    static size_t period_len = 0;
    static seq_t a0 = 0;

    if (period_len == 0) {
        const uint64_t D = 422ull;
        a0 = (seq_t)floor_sqrt((seq_t)D);

        uint64_t m = 0;
        uint64_t d = 1;
        uint64_t a_term = (uint64_t)a0;
        uint64_t a_next;
        const uint64_t a0_u = (uint64_t)a0;

        do {
            m = d * a_term - m;
            d = (D - m * m) / d;
            a_next = (a0_u + m) / d;
            period[period_len++] = (seq_t)a_next;
            a_term = a_next;
        } while (a_next != 2 * a0_u);
    }

    if (n == 0)
        return a0;

    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
