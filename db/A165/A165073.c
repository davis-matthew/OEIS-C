/*
 * A165073  Length of preperiodic part of trajectory of n under iteration of
 *          the base-7 Kaprekar map in A165071.
 * https://oeis.org/A165073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   length of the preperiodic part of the trajectory of n under
 *            iteration of the base‑7 Kaprekar map. The value is the index of
 *            the first repeated element in the orbit of n
 * Range:     u8 n <= 54, u16 n <= 2401, u32 n <= 2401, u64 n <= 2401,
 *            u128 n <= 2401, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Helper: one Kaprekar iterate in base 7. */
static seq_t kaprekar_next(seq_t x)
{
    unsigned int cnt[7] = {0};

    if (x == 0) {
        cnt[0] = 1;
    } else {
        seq_t t = x;
        while (t > 0) {
            unsigned int d = (unsigned int)(t % 7);
            ++cnt[d];
            t /= 7;
        }
    }

    seq_t mx = 0;
    for (int d = 6; d >= 0; --d) {
        for (unsigned int k = 0; k < cnt[d]; ++k) {
            mx = mx * (seq_t)7 + (seq_t)d;
        }
    }

    seq_t mn = 0;
    for (int d = 0; d <= 6; ++d) {
        for (unsigned int k = 0; k < cnt[d]; ++k) {
            mn = mn * (seq_t)7 + (seq_t)d;
        }
    }

    return mx - mn;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;

    /* A static array large enough for all preperiods of n <= 10000
       (empirical observation: preperiod length < 20000). */
    static seq_t seen[20000];
    size_t seen_len = 0;

    for (;;) {
        /* Look for repetition. */
        for (size_t j = 0; j < seen_len; ++j) {
            if (seen[j] == x) {
                return (seq_t)j;
            }
        }

        /* Store current value. */
        if (seen_len == 20000) { /* guard against runaway */
            /* Should never happen for the requested range. */
            return (seq_t)seen_len;
        }
        seen[seen_len++] = x;

        /* Next iterate. */
        x = kaprekar_next(x);
    }
}
