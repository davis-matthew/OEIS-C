/*
 * A216502  Indices of record values in A216476.
 * https://oeis.org/A216502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search -- scan the L-digit numbers m in increasing order and
 *            compute the Oware period A216476(m) by walking the cycle of
 *            pivot-relative digit words: period = q*L/gcd(shift,L). Each
 *            record period gives a term. If a smaller L-digit number lies on
 *            the same cycle, m cannot be a record, so the walk st...
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 49 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define A216502_MAXD 20
#define A216502_MAXREC 512
#define A216502_WINDOW ((uint64_t)1 << 28)

static uint64_t A216502_P10[A216502_MAXD + 1];
static bool A216502_init = false;

static uint64_t A216502_gcd(uint64_t x, uint64_t y) {
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

/* Period of m, or 0 if some smaller L-digit number lies on its cycle
   (then m cannot be a record). */
static uint64_t A216502_walk(uint64_t m, int L, uint64_t lo, uint64_t R,
                             uint8_t *seen, uint64_t win) {
    int c[A216502_MAXD];
    uint64_t x = m;
    for (int i = L - 1; i >= 0; --i) { c[i] = (int)(x % 10); x /= 10; }
    uint64_t V0 = m;      /* sum c[i]*10^(L-1-i), mod 2^64 */
    int bad = 0;          /* number of cells > 9 */
    int p = 0;
    uint64_t q = 0;
    for (;;) {
        int d = c[p];
        c[p] = 0;
        V0 -= (uint64_t)d * A216502_P10[L - 1 - p];
        if (d > 9) bad--;
        int r = d;
        if (d >= L) {
            int full = d / L;
            r = d % L;
            V0 += (uint64_t)full * R;
            bad = 0;
            for (int i = 0; i < L; ++i) {
                c[i] += full;
                if (c[i] > 9) bad++;
            }
        }
        int j = p;
        for (int k = 0; k < r; ++k) {
            if (++j == L) j = 0;
            if (++c[j] == 10) bad++;
            V0 += A216502_P10[L - 1 - j];
        }
        p = j;
        ++q;
        if (bad == 0) {
            uint64_t v = (V0 % A216502_P10[L - p]) * A216502_P10[p]
                       + V0 / A216502_P10[L - p];
            if (v == m) break;
            if (v >= lo) {
                if (v < m) return 0;
                uint64_t off = v - lo;
                if (off < win) seen[off >> 3] |= (uint8_t)(1u << (off & 7));
            }
        }
    }
    uint64_t g = A216502_gcd((uint64_t)p, (uint64_t)L);
    return q * ((uint64_t)L / g);
}

static uint64_t A216502_recs[A216502_MAXREC];
static size_t A216502_nrec = 0;
static uint64_t A216502_best = 0;
static int A216502_L = 0;
static uint64_t A216502_lo = 0, A216502_hi = 0, A216502_cur = 1;
static uint64_t A216502_R = 0, A216502_win = 0;
static uint8_t *A216502_seen = NULL;
static bool A216502_started = false;

seq_t a(idx_t n) {
    if (!A216502_init) {
        A216502_P10[0] = 1;
        for (int i = 1; i <= A216502_MAXD - 1; ++i)
            A216502_P10[i] = A216502_P10[i - 1] * 10;
        A216502_init = true;
    }
    size_t want = (size_t)n;
    if (want > A216502_MAXREC) want = A216502_MAXREC;
    while (A216502_nrec < want) {
        if (!A216502_started || A216502_cur > A216502_hi) {
            if (A216502_L >= 19) break;
            if (A216502_seen) { free(A216502_seen); A216502_seen = NULL; }
            A216502_L++;
            A216502_lo = A216502_P10[A216502_L - 1];
            A216502_hi = A216502_P10[A216502_L] - 1;
            A216502_R = (A216502_P10[A216502_L] - 1) / 9;
            uint64_t cnt = A216502_hi - A216502_lo + 1;
            A216502_win = cnt < A216502_WINDOW ? cnt : A216502_WINDOW;
            A216502_seen = (uint8_t *)calloc((size_t)((A216502_win + 7) / 8), 1);
            if (!A216502_seen) A216502_win = 0;
            A216502_cur = A216502_lo;
            A216502_started = true;
        }
        uint64_t m = A216502_cur;
        uint64_t off = m - A216502_lo;
        bool skip = (off < A216502_win) &&
                    (A216502_seen[off >> 3] & (1u << (off & 7)));
        if (!skip) {
            uint64_t per = A216502_walk(m, A216502_L, A216502_lo, A216502_R,
                                        A216502_seen, A216502_win);
            if (per > A216502_best) {
                A216502_best = per;
                A216502_recs[A216502_nrec++] = m;
            }
        }
        A216502_cur++;
    }
    if ((size_t)n == 0 || (size_t)n > A216502_nrec) return (seq_t)0;
    return (seq_t)A216502_recs[(size_t)n - 1];
}
