/*
 * A001292  Concatenations of cyclic permutations of initial positive
 *          integers.
 * https://oeis.org/A001292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) for n = k(k-1)/2 + j, 1 <= j <= k, is the j-th smallest of
 *            the k numbers formed by concatenating (r+1, r+2, ..., k, 1, ...,
 *            r) for r = 0..k-1. All of them have the same digit count, so the
 *            code orders them by comparing their digit strings and then
 *            builds the chosen one
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 45, u64 n <= 105, u128 n <= 293,
 *            big any n
 * Helpers:   rot_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/rot_cmp.h"

seq_t a(idx_t n) {
    idx_t k = 1;
    idx_t total = 0;
    while (total + k < n) {
        total += k;
        k++;
    }
    idx_t j = n - total; /* 1..k */

    /* digit string of 1,2,...,k concatenated */
    idx_t L = 0;
    for (idx_t m = 1; m <= k; m++) {
        idx_t x = m;
        while (x > 0) { L++; x /= 10; }
    }
    char *s = (char *)malloc((size_t)L + 1);
    idx_t *off = (idx_t *)malloc((size_t)k * sizeof(idx_t));
    idx_t pos = 0;
    for (idx_t m = 1; m <= k; m++) {
        char tmp[24];
        int len = 0;
        idx_t x = m;
        while (x > 0) { tmp[len++] = (char)('0' + (int)(x % 10)); x /= 10; }
        off[m - 1] = pos;
        while (len > 0) s[pos++] = tmp[--len];
    }

    /* stable insertion sort of rotation offsets by digit string */
    for (idx_t i = 1; i < k; i++) {
        idx_t cur = off[i];
        idx_t p = i;
        while (p > 0 && rot_cmp(s, L, off[p - 1], cur) > 0) {
            off[p] = off[p - 1];
            p--;
        }
        off[p] = cur;
    }

    idx_t start = off[j - 1];
    seq_t result = 0;
    for (idx_t t = 0; t < L; t++) {
        result = result * 10 + (seq_t)(s[(start + t) % L] - '0');
    }

    free(s);
    free(off);
    return result;
}
