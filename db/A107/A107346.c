/*
 * A107346  Differences between successive permutations of 1,2,3,4,5 regarded
 *          as decimal numbers arranged in increasing order.
 * https://oeis.org/A107346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, fini, full
 * Formula:   a(n) = difference between the n-th and (n-1)-th permutations *
 *            of the digits 1,2,3,4,5 interpreted as decimal numbers sorted in
 *            ascending order
 * Range:     u8 n <= 5, u16 n <= 119, u32 n <= 119, u64 n <= 119,
 *            u128 n <= 119, big any n
 * Validated: full, pass, 119 terms through n = 119 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static inline seq_t _perm_value(const int *p) {
    seq_t v = 0;
    v = v * 10 + p[0];
    v = v * 10 + p[1];
    v = v * 10 + p[2];
    v = v * 10 + p[3];
    v = v * 10 + p[4];
    return v;
}

static void _init_diffs(seq_t *diffs) {
    int perm[5] = {1, 2, 3, 4, 5};
    seq_t prev = _perm_value(perm);
    for (int count = 1; count < 120; ++count) {
        /* compute next lexicographic permutation of perm[0..4] */
        int i = 3;                     /* search for pivot where a[i] < a[i+1] */
        while (i >= 0 && perm[i] >= perm[i + 1]) --i;
        /* find rightmost element greater than pivot */
        int j = 4;
        while (perm[j] <= perm[i]) --j;
        /* swap pivot and successor */
        int tmp = perm[i];
        perm[i] = perm[j];
        perm[j] = tmp;
        /* reverse suffix after pivot */
        int l = i + 1, r = 4;
        while (l < r) {
            tmp = perm[l];
            perm[l] = perm[r];
            perm[r] = tmp;
            ++l;
            --r;
        }
        seq_t cur = _perm_value(perm);
        diffs[count - 1] = cur - prev;
        prev = cur;
    }
}

seq_t a(idx_t n) {
    static seq_t diffs[119] = {0};
    static int init = 0;
    if (!init) {
        _init_diffs(diffs);
        init = 1;
    }
    if (n == 0) return 0;
    if (n > 119) return 0;
    return diffs[n - 1];
}
