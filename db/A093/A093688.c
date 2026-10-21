/*
 * A093688  Numbers m such that all divisors of m, excluding the divisor 1,
 *          have an even number of 1's in their binary expansions.
 * https://oeis.org/A093688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th integer m such that every divisor d>1 of m has
 *            an even number of 1's in its binary expansion (search method,
 *            only odd m are tested)
 * Range:     u8 n <= 13, u16 n <= 974, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, popcount_even
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"
#include "../utils/popcount_even.h"

/* Test whether m satisfies the definition */
static bool is_valid(seq_t m)
{
    if (m == 1) return true;            /* 1 is trivially valid */

    /* m itself must be evil */
    if (!popcount_even(m)) return false;

    /* For odd m all divisors are odd; skip even divisors entirely. */
    seq_t sqrt_m = floor_sqrt(m);

    for (seq_t d = 3; d <= sqrt_m; d += 2) {
        if (m % d == 0) {
            if (!popcount_even(d)) return false;
            seq_t d2 = m / d;
            if (d2 != d && !popcount_even(d2)) return false;
        }
    }
    return true;
}

/* Return the nth term of the sequence (a(1)=1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for 0, return 0 */

    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static seq_t next_candidate = 1;

    while (seq_len < n) {
        if (is_valid(next_candidate)) {
            if (seq_len >= seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 64;
                seq = (seq_t *)realloc(seq, sizeof(seq_t) * seq_cap);
            }
            seq[seq_len++] = next_candidate;
        }

        /* Increment to the next odd integer */
        if (next_candidate == 1)
            next_candidate = 3;
        else
            next_candidate += 2;
    }

    return seq[n - 1];
}
