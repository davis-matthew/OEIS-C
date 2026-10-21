/*
 * A078825  Sum of distinct binary numbers contained as substrings in binary,
 *          circular formed representation of n and not longer than n.
 * https://oeis.org/A078825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = sum of distinct integers represented by circular binary substrings of n */
#include <stdlib.h>

/* comparison function for qsort on seq_t values */
static int cmp_seq(const void *a, const void *b) {
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* a(0) = 0 */

    /* compute bit length L of n */
    idx_t L = 0;
    idx_t tmp = n;
    while (tmp) { L++; tmp >>= 1; }

    /* bits array: most significant bit first */
    idx_t bits[64];
    for (idx_t i = 0; i < L; ++i) {
        bits[i] = (n >> (L - 1 - i)) & 1;
    }

    /* duplicate for convenient circular indexing */
    idx_t dbl[128];
    for (idx_t i = 0; i < 2 * L; ++i) {
        dbl[i] = bits[i % L];
    }

    /* maximum number of substrings = L * L (≤ 4096) */
    const size_t MAX_SUB = 4096;
    seq_t vals[MAX_SUB];
    size_t cnt = 0;

    /* enumerate all circular substrings and collect their integer values */
    for (idx_t len = 1; len <= L; ++len) {
        for (idx_t start = 0; start < L; ++start) {
            seq_t val = 0;
            for (idx_t k = 0; k < len; ++k) {
                val = (val << 1) | (seq_t)dbl[start + k];
            }
            vals[cnt++] = val;
        }
    }

    /* sort the values to find distinct ones */
    qsort(vals, cnt, sizeof(seq_t), cmp_seq);

    /* sum the distinct values */
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (i == 0 || vals[i] != vals[i - 1]) {
            sum += vals[i];
        }
    }

    return sum;
}
