/*
 * A216188  Number of unordered pairs of anagrammatic (positive) integers
 *          adding to n.
 * https://oeis.org/A216188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of unordered pairs of positive integers (x,y) such
 *            that * x + y = n and digit_counts(x) == digit_counts(y) (in base
 *            10)
 * Range:     u8 n <= 983, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_counts.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;
    seq_t count = 0;
    idx_t half = n / 2;
    unsigned char ca[10], cb[10];
    for (idx_t a = 1; a <= half; ++a) {
        idx_t b = n - a;
        digit_counts((seq_t)a, ca);
        digit_counts((seq_t)b, cb);
        bool same = true;
        for (int d = 0; d < 10; ++d) {
            if (ca[d] != cb[d]) { same = false; break; }
        }
        if (same) ++count;
    }
    return count;
}
