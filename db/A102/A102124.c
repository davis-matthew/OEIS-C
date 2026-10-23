/*
 * A102124  Iccanobirt numbers (14 of 15): a(n) = R(R(a(n-1)) + R(a(n-2)) +
 *          a(n-3)), where R is the digit reversal function A004086.
 * https://oeis.org/A102124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = R(R(a(n-1)) + R(a(n-2)) + a(n-3)) a(0) = 0, a(1) = 0,
 *            a(2) = 1
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 36, u64 n <= 69, u128 n <= 136,
 *            big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/rev.h"

seq_t a(idx_t n) {
    static seq_t *seq = NULL;
    static size_t cap = 0;
    static idx_t sz = 0;

    /* initial terms */
    if (sz == 0) {
        cap = 4;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        if (seq == NULL) return 0;            /* out of memory */
        seq[0] = 0;
        seq[1] = 0;
        seq[2] = 1;
        sz = 3;
    }

    if ((idx_t)n < sz) {
        return seq[(size_t)n];
    }

    /* extend storage to index n */
    size_t needed = (size_t)(n + 1);
    if (needed > cap) {
        size_t newcap = cap > 0 ? cap : 4;
        while (newcap < needed) newcap <<= 1;
        seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
        if (seq == NULL) return 0;            /* out of memory */
        cap = newcap;
    }

    for (idx_t i = sz; i <= n; ++i) {
        seq_t a1 = seq[(size_t)(i - 1)];
        seq_t a2 = seq[(size_t)(i - 2)];
        seq_t a3 = seq[(size_t)(i - 3)];
        seq[(size_t)i] = rev(rev(a1) + rev(a2) + a3);
    }
    sz = n + 1;
    return seq[(size_t)n];
}
