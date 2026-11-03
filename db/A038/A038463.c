/*
 * A038463  Sums of 12 distinct powers of 2.
 * https://oeis.org/A038463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of 12 distinct powers of 2 enumerated in increasing
 *            order
 * Range:     u8 none, u16 n <= 1820, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 51 terms through n = 9804 (2026-11-01)
 * Helpers:   binomial_u64_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/binomial_u64_u32.h"

seq_t a(idx_t n)
{
    const unsigned K = 12;
    unsigned long long rank = (unsigned long long)n - 1ULL; /* 0‑based rank */
    unsigned pos[K];

    for (unsigned i = K; i >= 1; --i) {
        unsigned low = 0;
        unsigned high = i;
        /* enlarge high until C(high,i) > rank */
        while (binomial_u64_u32(high, i) <= rank) {
            if (high > ULLONG_MAX / 2) {
                high = ULLONG_MAX;
                break;
            }
            high <<= 1;
        }

        unsigned best = 0;
        while (low <= high) {
            unsigned mid = low + ((high - low) >> 1);
            unsigned long long val = binomial_u64_u32(mid, i);
            if (val <= rank) {
                best = mid;
                low = mid + 1;
            } else {
                if (mid == 0) break;
                high = mid - 1;
            }
        }

        pos[i - 1] = best;
        rank -= binomial_u64_u32(best, i);
    }

    seq_t sum = 0;
    for (unsigned i = 0; i < K; ++i) {
        sum += ((seq_t)1 << pos[i]);
    }
    return sum;
}
