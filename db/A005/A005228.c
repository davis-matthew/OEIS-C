/*
 * A005228  Sequence and first differences (A030124) together list all
 *          positive numbers exactly once.
 * https://oeis.org/A005228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1) + c(n-1) for n >= 2, a(1) = 1, where c = A030124
 *            is the complement of a
 * Range:     u8 n <= 19, u16 n <= 347, u32 n <= 9487, u64 n <= 9487,
 *            u128 n <= 9487, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    static uint64_t *an = 0;   /* native copy of terms, used to build the complement */
    static seq_t *av = 0;      /* exact terms in seq_t */
    static idx_t cnt = 0, cap = 0;
    static uint64_t cval = 0;  /* last complement value produced */
    static idx_t j = 1;        /* index of smallest term not yet passed by cval */

    if (cnt == 0) {
        cap = 64;
        an = (uint64_t *)malloc((size_t)cap * sizeof(uint64_t));
        av = (seq_t *)malloc((size_t)cap * sizeof(seq_t));
        an[1] = 1;
        av[1] = 1;
        cnt = 1;
        cval = 0;
        j = 1;
    }

    if (n + 2 > cap) {
        idx_t ncap = cap;
        while (ncap < n + 2) ncap *= 2;
        uint64_t *nan = (uint64_t *)malloc((size_t)ncap * sizeof(uint64_t));
        seq_t *nav = (seq_t *)malloc((size_t)ncap * sizeof(seq_t));
        for (idx_t i = 1; i <= cnt; i++) {
            nan[i] = an[i];
            nav[i] = av[i];
        }
        free(an);
        free(av);
        an = nan;
        av = nav;
        cap = ncap;
    }

    while (cnt < n) {
        /* next complement value */
        cval++;
        while (j <= cnt && an[j] < cval) j++;
        while (j <= cnt && an[j] == cval) {
            cval++;
            j++;
        }
        an[cnt + 1] = an[cnt] + cval;
        av[cnt + 1] = av[cnt] + (seq_t)cval;
        cnt++;
    }

    return av[n];
}
