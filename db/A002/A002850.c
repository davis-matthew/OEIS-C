/*
 * A002850  Number of decompositions of 2n into sum of 2 lucky numbers.
 * https://oeis.org/A002850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = #{(i,j): i <= j, L(i) + L(j) = 2n}, L = lucky numbers
 *            (A000959); computed by a lucky-number sieve
 * Range:     u8 n <= 5854, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static bool *lk_flags = NULL;
static idx_t lk_limit = 0;

static void lk_build(idx_t max_num) {
    idx_t *list = (idx_t *)malloc((max_num / 2 + 2) * sizeof(idx_t));
    size_t cnt = 0;
    for (idx_t v = 1; v <= max_num; v += 2) list[cnt++] = v;

    size_t k = 1;
    while (k < cnt && list[k] <= cnt) {
        size_t step = (size_t)list[k];
        size_t w = 0;
        for (size_t i = 0; i < cnt; i++) {
            if ((i + 1) % step != 0) list[w++] = list[i];
        }
        cnt = w;
        k++;
    }

    bool *flags = (bool *)calloc(max_num + 1, sizeof(bool));
    for (size_t i = 0; i < cnt; i++) flags[list[i]] = true;
    free(list);

    free(lk_flags);
    lk_flags = flags;
    lk_limit = max_num;
}

seq_t a(idx_t n) {
    idx_t target = 2 * n;
    if (lk_flags == NULL || lk_limit < target) {
        idx_t lim = target < 1024 ? 1024 : target;
        lk_build(lim);
    }

    seq_t count = 0;
    for (idx_t L = 1; 2 * L <= target; L++) {
        if (lk_flags[L] && lk_flags[target - L]) count++;
    }
    return count;
}
