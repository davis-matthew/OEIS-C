/*
 * A134941  Mountain numbers.
 * https://oeis.org/A134941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 4241, u64 n <= 9577,
 *            u128 n <= 9577, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Mountain numbers: a(n) is the n‑th integer whose decimal representation
 * starts and ends with 1, contains a unique maximal digit (the peak) that
 * occurs only once, has digits before the peak strictly increasing,
 * and has digits after the peak strictly decreasing.
 *
 * Implementation: enumerate all possible peak digits (1..9) and for each
 * construct every arrangement of the digits 2..(peak‑1) that may appear
 * on the left side, on the right side, on both sides, or not at all.
 * Assemble the number 1…peak…1, cache all 21846 terms in sorted order,
 * and provide O(1) lookup for arbitrary n ≥ 1.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <algorithm>
#endif

static int seq_compare(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

static seq_t *mountain_cache = NULL;
static const idx_t MOUNTAIN_COUNT = 21846;

seq_t a(idx_t n)
{
    if (mountain_cache == NULL) {
        mountain_cache = (seq_t *)malloc(sizeof(seq_t) * MOUNTAIN_COUNT);
        idx_t pos = 0;

        /* first term */
        mountain_cache[pos++] = (seq_t)1;

        for (int m = 2; m <= 9; ++m) {
            int k = m - 2;               /* digits 2 .. m-1 */
            idx_t total = 1;
            for (int i = 0; i < k; ++i) total *= 4;   /* 4^k combinations */

            for (idx_t mask = 0; mask < total; ++mask) {
                seq_t val = (seq_t)1;   /* leading 1 */

                /* left side: digits 2 .. m-1 in increasing order */
                for (int d = 2; d <= m - 1; ++d) {
                    int state = (mask >> (2 * (d - 2))) & 3;
                    if (state == 0 || state == 2)   /* left or both */
                        val = val * (seq_t)10 + (seq_t)d;
                }

                /* peak digit */
                val = val * (seq_t)10 + (seq_t)m;

                /* right side: digits m-1 .. 2 in decreasing order */
                for (int d = m - 1; d >= 2; --d) {
                    int state = (mask >> (2 * (d - 2))) & 3;
                    if (state == 1 || state == 2)   /* right or both */
                        val = val * (seq_t)10 + (seq_t)d;
                }

                /* trailing 1 */
                val = val * (seq_t)10 + (seq_t)1;

                mountain_cache[pos++] = val;
            }
        }

        /* sort the generated values */
#ifdef __cplusplus
        std::sort(mountain_cache, mountain_cache + pos);
#else
        qsort(mountain_cache, pos, sizeof(seq_t), seq_compare);
#endif
    }
    return mountain_cache[n - 1];
}
