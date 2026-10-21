/*
 * A055157  n has distinct digits and n=a-b where a has the digits of n in
 *          descending order and b has the digits of n in ascending order
 *          (perhaps with leading zeros).
 * https://oeis.org/A055157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = A055158(n) - A055159(n) (descending minus ascending
 *            digits of n, n must have distinct digits)
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Small helper: popcount for ten‑bit masks */
static inline int popcount10(int mask) {
    int c = 0;
    while (mask) {
        c += mask & 1;
        mask >>= 1;
    }
    return c;
}

/* Comparator for qsort on seq_t (unsigned integer or arbitrary precision) */
static int cmp_seq(const void *p1, const void *p2) {
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Cache for all values of the sequence */
static seq_t *cache = NULL;
static size_t cache_cnt = 0;

/* Build the whole sequence once */
static void build_cache(void) {
    /* Temporary storage – we won't have more than a few hundred terms */
    seq_t tmp[1024];
    size_t len = 0;

    for (int mask = 1; mask < (1 << 10); ++mask) {
        int sz = popcount10(mask);
        if (sz < 2) continue;                     /* single‑digit sets give 0 */

        /* Construct a (descending digits) and b (ascending digits) */
        seq_t a = 0, b = 0;
        for (int d = 9; d >= 0; --d)
            if (mask & (1 << d))
                a = a * 10 + (seq_t)d;

        for (int d = 0; d <= 9; ++d)
            if (mask & (1 << d))
                b = b * 10 + (seq_t)d;

        seq_t diff = a - b;                       /* guaranteed non‑negative */
        if (diff == 0) continue;                  /* n must be positive */

        /* Determine the digit mask of diff, ensure all digits are distinct */
        seq_t tmpv = diff;
        int maskDiff = 0;
        bool ok = true;
        int digitsSeen = 0;
        while (ok && tmpv > 0) {
            int dg = (int)(tmpv % 10);
            if (maskDiff & (1 << dg))
                ok = false;                       /* repeated digit */
            else {
                maskDiff |= 1 << dg;
                ++digitsSeen;
            }
            tmpv /= 10;
        }
        if (!ok || digitsSeen != sz || maskDiff != mask)
            continue;                              /* invalid set */

        /* Store the valid term */
        tmp[len++] = diff;
    }

    /* Sort terms ascending */
    qsort(tmp, len, sizeof(seq_t), cmp_seq);

    /* Allocate cache and copy */
    cache = (seq_t *)malloc(len * sizeof(seq_t));
    for (size_t i = 0; i < len; ++i)
        cache[i] = tmp[i];
    cache_cnt = len;
}

/* Public function required by the build */
seq_t a(idx_t n) {
    if (!cache) build_cache();

    if ((size_t)n >= cache_cnt)
        return (seq_t)0;         /* out of bounds – undefined behaviour not needed */

    return cache[(size_t)n];
}
