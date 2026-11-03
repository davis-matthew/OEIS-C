/*
 * A216600  Number of ways to write n as a product with all factors <= n/4.
 * https://oeis.org/A216600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of unordered factorizations of n into factors all ≤
 *            floor(n/4) (at least two factors). The depth‑first search
 *            enumerates all such factorizations using ascending factor order
 *            to avoid duplicates
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 102 terms through n = 987 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t count_fact(idx_t rem, idx_t min_f, idx_t max_f)
{
    seq_t res = 0;
    /* iterate possible first factor f, ensuring f <= rem/f to keep
       order nondecreasing (ascending) and f <= max_f (global bound). */
    for (idx_t f = min_f; f <= max_f && f <= rem / f; ++f) {
        if (rem % f == 0) {
            idx_t rest = rem / f;
            /* count this factorization when the remaining product
               (rest) becomes the last factor. */
            if (rest <= max_f) {
                res += 1;
            }
            /* recurse to factor the rest further, respecting
               ascending order: next factor must be >= f. */
            if (rest >= f) {
                res += count_fact(rest, f, max_f);
            }
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t max_factor = n / 4;
    if (max_factor < 2) return 0;
    return count_fact(n, 2, max_factor);
}
