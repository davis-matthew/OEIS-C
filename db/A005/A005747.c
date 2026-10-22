/*
 * A005747  Number of n-covers of an unlabeled 6-set.
 * https://oeis.org/A005747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = B6(n) - B5(n), where Bk(n) = number of n X k binary
 *            matrices up to row and column permutations (zero rows/columns
 *            allowed). By Burnside, Bk(n) = (1/k!) * Sum_{sigma in S_k} [x^n]
 *            Prod_{orbits O of sigma on subsets of [k]} 1/(1 - x^|O|).
 *            Removing the zero columns shows that matrices with al...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 25, u128 n <= 79,
 *            big any n
 * Helpers:   add3, addmul3, alloc_zero, build_fixed, gen_classes, orbit_lens,
 *            release, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/add3.h"
#include "../utils/addmul3.h"
#include "../utils/build_fixed.h"
#include "../utils/gen_classes.h"
#include "../utils/orbit_lens.h"

static void build_big(int k, idx_t N, seq_t *tot) {
    int perm[16][6], cur[8], np = 0, c, lens[64];
    long cnt[16];
    idx_t i;
    seq_t *t = alloc_zero(N + 1);
    gen_classes(k, k, k, cur, 0, perm, cnt, &np);
    for (i = 0; i <= N; i++) tot[i] = 0;
    for (c = 0; c < np; c++) {
        int no = orbit_lens(k, perm[c], lens), o;
        for (i = 0; i <= N; i++) t[i] = 0;
        t[0] = 1;
        for (o = 0; o < no; o++) {
            idx_t L = (idx_t)lens[o];
            for (i = L; i <= N; i++) t[i] += t[i - L];
        }
        for (i = 0; i <= N; i++) tot[i] += t[i] * (seq_t)cnt[c];
    }
    release(t, N + 1);
}

static unsigned __int128 inv_odd(unsigned __int128 a) {
    unsigned __int128 x = a;
    int i;
    for (i = 0; i < 8; i++) x *= (unsigned __int128)2 - a * x;
    return x;
}

/* exact division of a 192-bit residue by 2^s * odd, result mod 2^128 */
static unsigned __int128 div_exact(const uint64_t *l, int s, unsigned __int128 odd) {
    uint64_t lo = (l[0] >> s) | (l[1] << (64 - s));
    uint64_t hi = (l[1] >> s) | (l[2] << (64 - s));
    unsigned __int128 v = ((unsigned __int128)hi << 64) | lo;
    return v * inv_odd(odd);
}

static idx_t have = 0;
static uint64_t *f6 = NULL, *f5 = NULL;
static seq_t *b6 = NULL, *b5 = NULL;

seq_t a(idx_t n) {
    int big = seq_is_big();
    if (n > have) {
        idx_t N = n;
        if (N < 2 * have) N = 2 * have;
        if (N < 64) N = 64;
        if (big) {
            if (b6) release(b6, have + 1);
            if (b5) release(b5, have + 1);
            b6 = alloc_zero(N + 1);
            b5 = alloc_zero(N + 1);
            build_big(6, N, b6);
            build_big(5, N, b5);
        } else {
            free(f6);
            free(f5);
            f6 = (uint64_t *)malloc(sizeof(uint64_t) * 3 * (size_t)(N + 1));
            f5 = (uint64_t *)malloc(sizeof(uint64_t) * 3 * (size_t)(N + 1));
            build_fixed(6, N, f6);
            build_fixed(5, N, f5);
        }
        have = N;
    }
    if (big) {
        return b6[n] / 720 - b5[n] / 120;
    } else {
        unsigned __int128 a6 = div_exact(f6 + 3 * n, 4, 45);
        unsigned __int128 a5 = div_exact(f5 + 3 * n, 3, 15);
        return u128_to_seq(a6 - a5);
    }
}
