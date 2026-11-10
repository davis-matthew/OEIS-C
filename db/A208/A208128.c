/*
 * A208128  Numbers n for which A210255(n)=3.
 * https://oeis.org/A208128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number k with A210255(k) = 3; computed by a sieve
 *            over m of the intervals (v2(m!), v2(m!)+v2(m)], doubling the
 *            limit until n terms are found
 * Range:     u8 n <= 7, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static idx_t *A208128_list = 0;
static idx_t A208128_found = 0;

static void A208128_build(idx_t L) {
    unsigned char *cnt = (unsigned char *)calloc((size_t)L + 2, 1);
    idx_t m;
    idx_t k;
    idx_t c = 0;
    if (!cnt) return;
    for (m = 1;; ++m) {
        idx_t D = m - (idx_t)__builtin_popcountll(m);
        idx_t t, j, hi;
        if (D >= L) break;
        t = (idx_t)__builtin_ctzll(m);
        hi = D + t;
        if (hi > L) hi = L;
        for (j = D + 1; j <= hi; ++j) cnt[j]++;
    }
    for (k = 1; k <= L; ++k) if (cnt[k] == 3) c++;
    free(A208128_list);
    A208128_list = (idx_t *)malloc(((size_t)c + 1) * sizeof(idx_t));
    if (!A208128_list) { free(cnt); A208128_found = 0; return; }
    c = 0;
    for (k = 1; k <= L; ++k) if (cnt[k] == 3) A208128_list[c++] = k;
    A208128_found = c;
    free(cnt);
}

seq_t a(idx_t n) {
    static idx_t L = 0;
    if (n == 0) return 0;
    while (n > A208128_found) {
        L = (L == 0) ? 4096 : L * 2;
        A208128_build(L);
        if (A208128_found == 0 && !A208128_list) return 0;
    }
    return (seq_t)A208128_list[n - 1];
}
