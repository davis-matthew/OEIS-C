/*
 * A006363  Number of antichains (or order ideals) in the poset B_4 X [n]; or
 *          size of the distributive lattice J(B_4 X [n]).
 * https://oeis.org/A006363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..16} s(k)*binomial(n+1,k), where s(k) is the
 *            number of strict chains of order ideals 0 = I_0 < I_1 < ... <
 *            I_k = B_4 in J(B_4) (order-preserving maps from B_4 onto [k]);
 *            s(k) is computed by DP over the 168 ideals of B_4
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 8, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint16_t a006363_ideals[168];
static int a006363_nid = 0;
static uint64_t a006363_s[17];
static bool a006363_init = false;

static void a006363_setup(void) {
    int x, b, i, j, k;
    uint32_t mask;
    static uint64_t cnt[17][168];
    for (mask = 0; mask < (1U << 16); mask++) {
        bool ok = true;
        for (x = 0; x < 16 && ok; x++) {
            if (!(mask & (1U << x))) continue;
            for (b = 0; b < 4; b++) {
                if ((x >> b) & 1) {
                    int y = x ^ (1 << b);
                    if (!(mask & (1U << y))) { ok = false; break; }
                }
            }
        }
        if (ok) a006363_ideals[a006363_nid++] = (uint16_t)mask;
    }
    /* ideals are in increasing numeric order, so subsets come first */
    for (k = 0; k <= 16; k++)
        for (i = 0; i < a006363_nid; i++) cnt[k][i] = 0;
    cnt[0][0] = 1; /* empty ideal */
    for (k = 1; k <= 16; k++) {
        for (i = 0; i < a006363_nid; i++) {
            uint64_t s = 0;
            for (j = 0; j < i; j++) {
                if ((a006363_ideals[j] & a006363_ideals[i]) == a006363_ideals[j])
                    s += cnt[k - 1][j];
            }
            cnt[k][i] = s;
        }
    }
    for (k = 0; k <= 16; k++) a006363_s[k] = cnt[k][a006363_nid - 1];
    a006363_init = true;
}

seq_t a(idx_t n) {
    seq_t total = 0;
    seq_t c = 1; /* binomial(n+1, k) */
    seq_t m = (seq_t)n + 1;
    idx_t k;
    if (!a006363_init) a006363_setup();
    for (k = 1; k <= 16; k++) {
        if ((idx_t)(k - 1) > n) break;
        c = c * (m - (seq_t)(k - 1)) / (seq_t)k;
        total += (seq_t)a006363_s[k] * c;
    }
    return total;
}
