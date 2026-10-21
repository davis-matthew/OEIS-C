/*
 * A216601  Number of ways to write n as a product with all factors <= n/5.
 * https://oeis.org/A216601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of unordered factorizations of n into factors between 2 and floor(n/5) */
#include <stdint.h>
#include <stddef.h>

/* Recursive helper: count factorizations of n with
   smallest factor >= start and all factors <= limit
   (factors are processed in non‑decreasing order). */
static seq_t count_fact(idx_t n, idx_t start, idx_t limit)
{
    seq_t total = 0;
    for (idx_t f = start; f <= limit && f <= n / f; ++f) {
        if (n % f == 0) {
            idx_t rem = n / f;
            if (rem <= limit) {
                /* pair (f, rem) */
                ++total;
            }
            /* further factorize rem, keeping non‑decreasing order */
            if (rem > f) {
                total += count_fact(rem, f, limit);
            }
        }
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n < 5) {
        return 0;
    }
    idx_t limit = n / 5;
    if (limit < 2) {
        return 0;
    }
    return count_fact(n, 2, limit);
}
