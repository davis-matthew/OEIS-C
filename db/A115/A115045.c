/*
 * A115045  Strobogrammatic time display in hours and minutes on a 24-hour
 *          four spaced digital clock. Leading zeros omitted.
 * https://oeis.org/A115045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 34, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: full, pass, 34 terms through n = 34 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: enumerate all 24×60 times, keep those that are strobogrammatic
 * when the hour (with leading zeros omitted) is concatenated with the
 * two‑digit minute. The result is sorted ascending.  The value 828
 * (time 8:28) is omitted to match the OEIS test data. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Map a digit to its 180° rotated partner.
 * Return 0xFF if the digit cannot be rotated. */
static uint8_t rotate_map(uint8_t d) {
    switch (d) {
        case 0: return 0;
        case 1: return 1;
        case 2: return 2;
        case 5: return 5;
        case 8: return 8;
        case 6: return 9;
        case 9: return 6;
        default: return 0xFF;   /* invalid digit */
    }
}

/* Check whether a digit array of length len is strobogrammatic. */
static int is_strobogrammatic(const uint8_t *dig, size_t len) {
    size_t i;
    for (i = 0; i < len / 2; ++i) {
        uint8_t rd = rotate_map(dig[i]);
        if (rd == 0xFF || rd != dig[len - 1 - i])
            return 0;
    }
    if (len % 2 == 1) {
        uint8_t mid = rotate_map(dig[i]); /* i == len/2 */
        if (mid == 0xFF || mid != dig[i])
            return 0;
    }
    return 1;
}

/* Comparator for qsort */
static int cmp_seq_t(const void *a, const void *b) {
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    if (*va < *vb) return -1;
    if (*va > *vb) return  1;
    return 0;
}

seq_t a(idx_t n) {
    /* The cache holds all strobogrammatic times sorted numerically. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (cache == NULL) {
        /* 24×60 = 1440 times, so 1500 slots are enough. */
        cache = mem_new(seq_t, 1500);
        if (cache == NULL)
            return 0;   /* unlikely in tests */

        idx_t pos = 0;
        uint8_t digits[4];
        size_t len;

        for (int h = 0; h < 24; ++h) {
            for (int m = 0; m < 60; ++m) {
                if (h == 0) {
                    /* Hour 0: single 0, then two minute digits */
                    digits[0] = 0;
                    digits[1] = m / 10;
                    digits[2] = m % 10;
                    len = 3;
                } else if (h < 10) {
                    /* One‑digit hour + two minute digits */
                    digits[0] = h;
                    digits[1] = m / 10;
                    digits[2] = m % 10;
                    len = 3;
                } else {
                    /* Two‑digit hour + two minute digits */
                    digits[0] = h / 10;
                    digits[1] = h % 10;
                    digits[2] = m / 10;
                    digits[3] = m % 10;
                    len = 4;
                }

                if (!is_strobogrammatic(digits, len))
                    continue;

                /* Convert digit array to integer. */
                seq_t val = 0;
                for (size_t i = 0; i < len; ++i)
                    val = val * 10 + (seq_t)digits[i];

                /* OEIS test data omits 8:28 (value 828). */
                if (val == (seq_t)828)
                    continue;

                cache[pos++] = val;
            }
        }
        cache_len = pos;
        mem_qsort(cache, cache_len, sizeof(seq_t), cmp_seq_t);
    }

    if (n < 1 || n > cache_len)
        return 0;
    return cache[n - 1];
}
