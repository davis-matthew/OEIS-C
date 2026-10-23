/*
 * A172218  Number of ways to place 3 nonattacking nightriders on a 3 X n
 *          board.
 * https://oeis.org/A172218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9n^3 - 57n^2 + 210n - 344)/2 for n ≥ 8, * computed by
 *            direct enumeration for n < 8
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 908, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   conflict_dir
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/conflict_dir.h"

static seq_t compute_small(idx_t n) {
    int N = 3 * (int)n;
    int row[21], col[21];
    for (int c = 0; c < (int)n; ++c)
        for (int r = 0; r < 3; ++r) {
            int idx = c*3 + r;
            row[idx] = r;
            col[idx] = c;
        }
    seq_t count = 0;
    for (int i = 0; i < N - 2; ++i) {
        for (int j = i + 1; j < N - 1; ++j) {
            if (conflict_dir(row[i], col[i], row[j], col[j])) continue;
            for (int k = j + 1; k < N; ++k) {
                if (conflict_dir(row[i], col[i], row[k], col[k]) ||
                    conflict_dir(row[j], col[j], row[k], col[k]))
                    continue;
                ++count;
            }
        }
    }
    return count;
}

seq_t a(idx_t n) {
    if (n < 8) {
        return compute_small(n);
    }
    seq_t ns = (seq_t)n;
    seq_t num = (seq_t)9 * ns * ns * ns - (seq_t)57 * ns * ns
              + (seq_t)210 * ns - (seq_t)344;
    return num / (seq_t)2;
}
