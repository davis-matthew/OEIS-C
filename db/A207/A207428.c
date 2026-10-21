/*
 * A207428  Number of 4Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A207428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4×n binary arrays avoiding horizontal triples
 *            000 and 010, and vertical triples 001 and 010 (translated to the
 *            vertical forbidden patterns 100 and 010) computed by a
 *            transfer‑matrix over the 9 allowed column patterns
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* List of 4‑bit column patterns that satisfy the vertical
       constraints (no 100 or 010 as a consecutive triple). */
    static const int patterns[9] = {0, 1, 3, 6, 7, 11, 13, 14, 15};

    /* Transition table: allowed[i][j][k] is true if a column
       pattern k can follow column pattern j after i,
       while respecting horizontal constraints for all 4 rows. */
    static bool allowed[9][9][9];
    static bool init = false;

    if (!init) {
        for (int i = 0; i < 9; ++i) {
            int x = patterns[i];
            for (int j = 0; j < 9; ++j) {
                int y = patterns[j];
                for (int k = 0; k < 9; ++k) {
                    int z = patterns[k];
                    bool ok = true;
                    for (int r = 0; r < 4; ++r) {
                        int mask = 1 << (3 - r);          /* bit positions top→bottom */
                        int a = (x & mask) != 0;
                        int b = (y & mask) != 0;
                        int c = (z & mask) != 0;
                        if ((a == 0 && b == 0 && c == 0) ||
                            (a == 0 && b == 1 && c == 0)) {
                            ok = false;
                            break;
                        }
                    }
                    allowed[i][j][k] = ok;
                }
            }
        }
        init = true;
    }

    if (n == 0) return (seq_t)0;          /* Undefined for n<1, safe to return 0 */
    if (n == 1) return (seq_t)9;          /* Three rows (top, middle, bottom) per column,
                                           but only 9 valid column patterns */
    /* DP arrays: cur[i][j] holds number of ways to obtain column pair
       (patterns[i], patterns[j]) as the last two columns of a partial array. */
    seq_t cur[9][9], next[9][9];
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            cur[i][j] = (seq_t)1;          /* For n = 2, all 9×9 pairs are possible */

    for (idx_t len = 3; len <= n; ++len) {
        /* Clear next array */
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                next[i][j] = (seq_t)0;
        /* Transition */
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                seq_t cnt = cur[i][j];
                if (cnt == 0) continue;
                for (int k = 0; k < 9; ++k) {
                    if (allowed[i][j][k])
                        next[j][k] += cnt;
                }
            }
        }
        /* Copy next to cur for the next iteration */
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                cur[i][j] = next[i][j];
    }

    /* Sum over all ending pairs to obtain total number of arrays. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            total += cur[i][j];
    return total;
}
