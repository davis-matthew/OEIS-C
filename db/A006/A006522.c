/*
 * A006522  4-dimensional analog of centered polygonal numbers. Also number of
 *          regions created by sides and diagonals of a convex n-gon in
 *          general position.
 * https://oeis.org/A006522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = binomial(n,4) + binomial(n-1,2), with a(0) = 1
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 568, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 930 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* binomial(m, k) for k <= 4: divide k! out of the factors natively, then multiply in seq_t */
static seq_t small_choose(uint64_t m, int k) {
    uint64_t f[4];
    int i, need2, need3;
    seq_t r = 1;
    if (m < (uint64_t)k) return 0;
    for (i = 0; i < k; i++) f[i] = m - (uint64_t)i;
    /* v_2(k!) and v_3(k!) for k <= 4 */
    need2 = (k >= 4) ? 3 : (k >= 2 ? 1 : 0);
    need3 = (k >= 3) ? 1 : 0;
    for (i = 0; i < k && need2 > 0; i++)
        while (need2 > 0 && f[i] % 2 == 0) { f[i] /= 2; need2--; }
    for (i = 0; i < k && need3 > 0; i++)
        while (need3 > 0 && f[i] % 3 == 0) { f[i] /= 3; need3--; }
    for (i = 0; i < k; i++) r = (seq_t)(r * (seq_t)f[i]);
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    return small_choose(n, 4) + small_choose(n - 1, 2);
}
