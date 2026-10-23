/*
 * A023778  Metadromes: digits in base 9 are in strict ascending order.
 * https://oeis.org/A023778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   a(n) = nth integer whose base-9 digits are in strictly
 *            increasing order
 * Range:     u8 n <= 1, u16 n <= 189, u32 n <= 256, u64 n <= 256,
 *            u128 n <= 256, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static int seq_cmp(const void *p1, const void *p2) {
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    return (*a < *b) ? -1 : ((*a > *b) ? 1 : 0);
}

seq_t a(idx_t n) {
    static seq_t cache[256];
    static int cached = 0;
    if (!cached) {
        size_t pos = 0;
        cache[pos++] = 0;                  /* single digit 0 */
        /* all non-empty subsets of digits 1..8 (bits 0..7) */
        for (int mask = 1; mask < (1 << 8); ++mask) {
            seq_t val = 0;
            for (int bit = 0; bit < 8; ++bit) {
                if (mask & (1 << bit)) {
                    val = val * 9 + (seq_t)(bit + 1);
                }
            }
            cache[pos++] = val;
        }
        /* pos should be 256 */
        qsort(cache, 256, sizeof(seq_t), seq_cmp);
        cached = 1;
    }
    idx_t idx = n - 1;
    if (idx >= 256) {
        return cache[255]; /* value beyond the sequence; return last known */
    }
    return cache[idx];
}
