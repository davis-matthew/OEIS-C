/*
 * A085123  Smallest member of A002473 whose digital product = A002473(n), or
 *          0 if no such number exists.
 * https://oeis.org/A085123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, base
 * Formula:   a(n) = smallest 7-smooth k (A002473) whose digit product equals
 *            A002473(n), else 0; found by searching 7-smooth numbers in
 *            increasing order up to 10^18 (or the seq_t limit)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A085123_list = NULL;
static idx_t A085123_len = 0;

static bool A085123_dp_eq(seq_t k, seq_t m) {
    seq_t p = 1;
    while (k > 0) {
        int d = (int)(k % 10);
        if (d == 0) return false;
        p *= d;
        if (p > m) return false;
        k /= 10;
    }
    return p == m;
}

static void A085123_build(void) {
    idx_t cap = 1024, i2 = 0, i3 = 0, i5 = 0, i7 = 0;
    seq_t lim = 1;
    for (int i = 0; i < 18; i++) {
        seq_t t = lim * 10;
        if (t / 10 != lim) break;
        lim = t;
    }
    seq_t *s = mem_new(seq_t, cap);
    s[0] = 1;
    idx_t len = 1;
    for (;;) {
        seq_t c[4];
        bool ok[4];
        c[0] = s[i2] * 2; ok[0] = (c[0] / 2 == s[i2]) && c[0] <= lim;
        c[1] = s[i3] * 3; ok[1] = (c[1] / 3 == s[i3]) && c[1] <= lim;
        c[2] = s[i5] * 5; ok[2] = (c[2] / 5 == s[i5]) && c[2] <= lim;
        c[3] = s[i7] * 7; ok[3] = (c[3] / 7 == s[i7]) && c[3] <= lim;
        bool have = false;
        seq_t best = 0;
        for (int j = 0; j < 4; j++) {
            if (ok[j] && (!have || c[j] < best)) { best = c[j]; have = true; }
        }
        if (!have) break;
        if (len >= cap) {
            idx_t nc = cap * 2;
            seq_t *ns = mem_new(seq_t, nc);
            for (idx_t i = 0; i < len; i++) ns[i] = s[i];
            mem_free(s);
            s = ns;
            cap = nc;
        }
        s[len++] = best;
        if (ok[0] && c[0] == best) i2++;
        if (ok[1] && c[1] == best) i3++;
        if (ok[2] && c[2] == best) i5++;
        if (ok[3] && c[3] == best) i7++;
    }
    A085123_list = s;
    A085123_len = len;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (A085123_list == NULL) A085123_build();
    if (n > A085123_len) return 0;
    seq_t m = A085123_list[n - 1];
    for (idx_t i = 0; i < A085123_len; i++) {
        if (A085123_dp_eq(A085123_list[i], m)) return A085123_list[i];
    }
    return 0;
}
