/*
 * A023808  Xenodromes: all digits in base 14 are different.
 * https://oeis.org/A023808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, easy
 * Formula:   a(n) = smallest non‑negative integer whose base‑14 digits are
 *            all different (the nth such integer, with a(1)=0)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   factorial_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factorial_factor.h"

#define B 14   /* base */

seq_t a(idx_t n)
{
    /* Single‑digit numbers first (0 … 13) */
    if (n <= B)
        return (seq_t)(n - 1);

    /* Rank among numbers with at least two digits */
    uint64_t rank = n - B;
    uint64_t count_len = 0;
    uint64_t len = 2;          /* minimal length for remaining numbers */

    /* Find the length L for which the n‑th number occurs */
    while (1) {
        count_len = (uint64_t)(B - 1) * factorial_factor(B - 1, len - 1);
        if (rank <= count_len)
            break;
        rank -= count_len;
        ++len;
    }

    /* Unrank the number of length ‘len’ at position ‘rank’ */
    bool used[B] = { false };
    seq_t value = 0;

    /* Most significant digit cannot be 0 */
    for (int d = 1; d < B; ++d) {
        uint64_t cnt = factorial_factor(B - 1, len - 1);
        if (rank > cnt) {
            rank -= cnt;
            continue;
        }
        value = (seq_t)d;
        used[d] = true;
        break;
    }

    /* Remaining positions */
    for (uint64_t pos = 1; pos < len; ++pos) {
        uint64_t rempos = len - pos - 1;
        int available = B - pos;    /* digits still unused (0 included) */

        for (int d = 0; d < B; ++d) {
            if (used[d])
                continue;

            uint64_t cnt = (rempos == 0) ?
                           1ULL :
                           factorial_factor(available - 1, rempos);

            if (rank > cnt) {
                rank -= cnt;
                continue;
            }

            value = value * (seq_t)B + (seq_t)d;
            used[d] = true;
            break;
        }
    }

    return value;
}
