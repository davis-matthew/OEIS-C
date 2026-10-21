/*
 * A211120  One of 17 possible sets of eleven numbers of the form 3^alpha
 *          5^beta 7^gamma whose sum of reciprocals is 1.
 * https://oeis.org/A211120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   add_fraction, gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: compute the lexicographically smallest set of eleven numbers
   of the form 3^α 5^β 7^γ whose reciprocals sum to 1, and return the n‑th
   element of that set (sorted increasingly). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/add_fraction.h"
#include "../utils/gcd_seq.h"

/* Euclidean algorithm for gcd, works for any unsigned seq_t */

/* Count set bits of a 32‑bit integer. */
static int popcount32(unsigned int x)
{
    int c = 0;
    while (x) {
        c += x & 1U;
        x >>= 1U;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* Static state holds the solved set once. */
    static bool ready = false;
    static seq_t sol[11];

    if (!ready) {
        /* Generate all 3^α 5^β 7^γ with α≤2, β≤2, γ≤1, excluding 1. */
        seq_t numbers[17];
        int pos = 0;
        for (unsigned int a = 0; a <= 2; ++a) {
            seq_t pa = 1;
            for (unsigned int i = 0; i < a; ++i) pa *= 3;
            for (unsigned int b = 0; b <= 2; ++b) {
                seq_t pb = 1;
                for (unsigned int i = 0; i < b; ++i) pb *= 5;
                for (unsigned int c = 0; c <= 1; ++c) {
                    if (a == 0 && b == 0 && c == 0) continue; /* skip 1 */
                    seq_t pc = (c == 1 ? 7 : 1);
                    numbers[pos++] = pa * pb * pc;
                }
            }
        }

        /* Sort the 17 numbers (insertion sort, 17 elements). */
        for (int i = 1; i < 17; ++i) {
            seq_t key = numbers[i];
            int j = i - 1;
            while (j >= 0 && numbers[j] > key) {
                numbers[j + 1] = numbers[j];
                --j;
            }
            numbers[j + 1] = key;
        }

        /* Search all 11‑element subsets for reciprocal sum == 1. */
        seq_t best[11];
        bool found = false;
        for (unsigned int mask = 0; mask < (1u << 17); ++mask) {
            if (popcount32(mask) != 11) continue;
            seq_t num = 0, den = 1;
            seq_t subset[11];
            int idx = 0;
            for (int i = 0; i < 17; ++i) {
                if (mask & (1u << i)) {
                    add_fraction(&num, &den, numbers[i]);
                    subset[idx++] = numbers[i];
                }
            }
            if (num == den) {
                if (!found) {
                    for (int i = 0; i < 11; ++i) best[i] = subset[i];
                    found = true;
                } else {
                    /* Keep lexicographically smallest set. */
                    bool better = false;
                    for (int i = 0; i < 11; ++i) {
                        if (subset[i] < best[i]) { better = true; break; }
                        if (subset[i] > best[i]) break;
                    }
                    if (better) {
                        for (int i = 0; i < 11; ++i) best[i] = subset[i];
                    }
                }
            }
        }

        if (found) {
            for (int i = 0; i < 11; ++i) sol[i] = best[i];
            ready = true;
        }
    }

    if (!ready) return 0;
    if (n >= 1 && n <= 11) {
        size_t idx = (size_t)(n - 1);
        return sol[idx];
    }
    return 0;
}
