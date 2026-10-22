/*
 * A000787  Strobogrammatic numbers: the same upside down.
 * https://oeis.org/A000787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   direct construction (no search). Length-1 terms are 0, 1, 8. For
 *            length L >= 2 there are 4*5^(floor(L/2)-1)*(L odd ? 3 : 1)
 *            terms. In numeric order, the k-th one (0-based) is k written in
 *            a mixed radix: the first digit is chosen from {1,6,8,9}, the
 *            inner left-half digits from {0,1,6,8,9}, and the...
 * Range:     u8 n <= 10, u16 n <= 60, u32 n <= 3124, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a787_rot
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a787_rot.h"

static const int a787_first[4] = {1, 6, 8, 9};
static const int a787_inner[5] = {0, 1, 6, 8, 9};
static const int a787_cen[3] = {0, 1, 8};

seq_t a(idx_t n) {
    idx_t r = n - 1;
    if (r < 3) return (seq_t)a787_cen[r];
    r -= 3;
    int L = 2;
    for (;;) {
        int h = L / 2;
        unsigned __int128 cnt = 4;
        for (int i = 1; i < h; i++) cnt *= 5;
        if (L % 2) cnt *= 3;
        if ((unsigned __int128)r < cnt) break;
        r -= (idx_t)cnt;
        L++;
    }
    int dig[160];
    int h = L / 2;
    idx_t k = r;
    if (L % 2) {
        dig[h] = a787_cen[k % 3];
        k /= 3;
    }
    for (int i = h - 1; i >= 1; i--) {
        int d = a787_inner[k % 5];
        k /= 5;
        dig[i] = d;
        dig[L - 1 - i] = a787_rot(d);
    }
    dig[0] = a787_first[k];
    dig[L - 1] = a787_rot(dig[0]);
    seq_t v = 0;
    for (int i = 0; i < L; i++) v = v * 10 + dig[i];
    return v;
}
