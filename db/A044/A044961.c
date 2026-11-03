/*
 * A044961  Numbers with no two equally numerous base 12 digits.
 * https://oeis.org/A044961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 798 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Series: numbers with distinct base-12 digit counts */

#include <stdbool.h>

/* Return true if the base‑12 representation of x has all
   digit frequencies that appear (i.e., >0) pairwise distinct. */
static bool distinct_counts(seq_t x)
{
    unsigned counts[12] = {0};
    while (x > 0) {
        unsigned d = (unsigned)(x % (seq_t)12);
        counts[d]++;
        x /= (seq_t)12;
    }
    unsigned freq[12] = {0};
    int m = 0;
    for (int i = 0; i < 12; ++i) {
        if (counts[i] > 0) {
            freq[m++] = counts[i];
        }
    }
    for (int i = 0; i < m; ++i) {
        for (int j = i + 1; j < m; ++j) {
            if (freq[i] == freq[j]) return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t seen = 0;
    seq_t cand = 1;
    while (1) {
        if (distinct_counts(cand)) {
            ++seen;
            if (seen == n)
                return cand;
        }
        ++cand;
    }
}
