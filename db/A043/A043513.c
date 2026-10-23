/*
 * A043513  Numbers having one 6 in base 10.
 * https://oeis.org/A043513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Numbers having exactly one digit equal to 6 in base 10
 * Range:     u8 n <= 41, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   pow9_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th smallest positive integer whose decimal expansion
 * contains exactly one digit 6. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow9_cap.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined, but safe */

    /* Find the number of digits d of the n-th term. */
    idx_t cumulative = 0;
    idx_t d = 0;
    idx_t offset = 0;
    idx_t limit = (n == UINT64_MAX) ? UINT64_MAX : n + 1; /* clamp target */

    while (1) {
        ++d;
        idx_t count_d;
        if (d == 1) {
            count_d = 1;
        } else {
            /* count_d = 9^(d-2) * (8*d + 1)  (clamped) */
            idx_t pow9 = pow9_cap(d - 2, limit);
            idx_t mult = 8 * d + 1;
            if (pow9 > limit / mult) count_d = limit;
            else count_d = pow9 * mult;
        }

        if (cumulative + count_d >= n) {
            offset = n - cumulative - 1;   /* zero‑based offset */
            break;
        }
        cumulative += count_d;
    }

    /* Construct the number digit by digit. */
    seq_t result = 0;
    bool seen6 = false;

    for (idx_t pos = 0; pos < d; ++pos) {
        idx_t rest = d - pos - 1;

        /* Pre‑compute powers of 9 needed for this position. */
        idx_t pow9_rest     = pow9_cap(rest,     limit);
        idx_t pow9_rest_1   = (rest >= 1 ? pow9_cap(rest - 1, limit) : 1);

        int low_digit = (pos == 0) ? 1 : 0;

        for (int digit = low_digit; digit <= 9; ++digit) {
            if (digit == 6 && seen6) continue;   /* would give two 6's */

            idx_t comps = 0;

            if (digit == 6) {
                comps = pow9_rest;               /* no further 6's allowed */
            } else {
                if (!seen6) {                    /* still need a 6 later */
                    if (rest == 0) {
                        comps = 0;                /* impossible */
                    } else {
                        if (pow9_rest_1 > limit / rest) comps = limit;
                        else comps = pow9_rest_1 * rest;
                    }
                } else {                        /* already seen a 6 */
                    comps = pow9_rest;
                }
            }

            if (comps == 0) continue;           /* this digit yields no match */

            if (offset >= comps) {
                offset -= comps;                 /* skip all numbers with this digit */
                continue;
            }

            /* This digit is the correct one. */
            result = result * (seq_t)10 + (seq_t)digit;
            if (digit == 6) seen6 = true;
            break;
        }
    }

    return result;
}
