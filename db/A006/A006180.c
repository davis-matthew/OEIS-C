/*
 * A006180  Witt vector *5!/5!.
 * https://oeis.org/A006180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   ghost components w(n) = (5n)!/(5!*(n!)^5) = Sum_{d|n}
 *            d*a(d)^(n/d), so a(n) = (w(n) - Sum_{d|n, d<n} d*a(d)^(n/d)) /
 *            n, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   grow_zero, seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/u128_to_seq.h"
#include "../utils/seq_is_fixed_width.h"

static char *sv_comp = NULL;
static idx_t sv_len = 0;

static seq_t *memo = NULL;
static bool *have = NULL;
static idx_t cap = 0;

static void ensure_sieve(idx_t N) {
    idx_t i, j;
    if (N + 1 <= sv_len) return;
    free(sv_comp);
    sv_comp = (char *)calloc(N + 1, 1);
    sv_len = N + 1;
    for (i = 2; i * i <= N; i++)
        if (!sv_comp[i])
            for (j = i * i; j <= N; j += i) sv_comp[j] = 1;
}

static uint64_t leg(uint64_t N, uint64_t p) {
    uint64_t e = 0;
    while (N) { N /= p; e += N; }
    return e;
}

static seq_t wval(idx_t n) {
    idx_t N = 5 * n, p;
    seq_t r = 1;
    uint64_t acc = 1;
    ensure_sieve(N);
    for (p = 2; p <= N; p++) {
        uint64_t e;
        if (sv_comp[p]) continue;
        e = leg(N, p) - 5 * leg(n, p);
        if (p == 2) e -= 3;
        else if (p == 3 || p == 5) e -= 1;
        while (e--) {
            if (acc > ((uint64_t)1 << 32)) { r *= acc; acc = 1; }
            acc *= p;
        }
    }
    r *= acc;
    return r;
}

static seq_t spow(seq_t b, idx_t e) {
    seq_t r = 1;
    while (e) {
        if (e & 1) r *= b;
        e >>= 1;
        if (e) b *= b;
    }
    return r;
}

static seq_t getc(idx_t n) {
    idx_t d;
    seq_t s = 0, D, c;
    if (n < cap && have[n]) return memo[n];
    for (d = 1; d < n; d++)
        if (n % d == 0)
            s += (seq_t)d * spow(getc(d), n / d);
    D = wval(n) - s;
    if (seq_is_fixed_width()) {
        idx_t m = n;
        int v = 0, i;
        unsigned __int128 x, mm;
        while ((m & 1) == 0) { m >>= 1; v++; }
        mm = m;
        x = mm;
        for (i = 0; i < 7; i++) x *= (unsigned __int128)2 - mm * x;
        c = (D >> v) * u128_to_seq(x);
    } else {
        c = D / (seq_t)n;
    }
    if (n >= cap) {
        idx_t nc = cap ? cap : 16;
        while (nc <= n) nc *= 2;
        memo = grow_zero(memo, cap, nc);
        have = (bool *)realloc(have, nc * sizeof(bool));
        memset(have + cap, 0, (nc - cap) * sizeof(bool));
        cap = nc;
    }
    memo[n] = c;
    have[n] = true;
    return c;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    return getc(n);
}
