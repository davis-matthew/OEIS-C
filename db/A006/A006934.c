/*
 * A006934  A series for Pi.
 * https://oeis.org/A006934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   k*a(k) = Sum_{j=1..k} |E(2j)| * 4^(j-1) * 2^(v2(k!) -
 *            v2((k-j)!)) * a(k-j), a(0) = 1, where E(2j) are the Euler
 *            (secant) numbers A000364. This follows from log(Sum_k c(k)
 *            m^(-2k)) = Sum_{j>=1} E(2j)/(4j) m^(-2j), with c(k) = (-1)^k
 *            a(k)/A088802(k) and A088802(k) = 4^k * 2^v2(k!)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, seq_is_big, u128_to_seq, v2f
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/v2f.h"

/* ---------- fixed-width path: exact 2-adic arithmetic mod 2^128 ---------- */
typedef unsigned __int128 u128_t;

static seq_t fixed_a(idx_t n) {
    u128_t rows[2][132], E[67], A[67];
    int cur = 0;
    idx_t m, k, j;
    if (n > 64) return 0; /* value cannot fit any fixed-width seq_t */
    rows[0][0] = 1; E[0] = 1;
    for (m = 0; m < 2 * n; m++) {
        u128_t *o = rows[cur], *r = rows[cur ^ 1];
        r[0] = 0;
        for (k = 1; k <= m + 1; k++) r[k] = r[k - 1] + o[m + 1 - k];
        cur ^= 1;
        if (!((m + 1) & 1)) E[(m + 1) / 2] = r[m + 1];
    }
    A[0] = 1;
    for (k = 1; k <= n; k++) {
        u128_t sum = 0, o, inv;
        idx_t t = 0, uk = v2f(k), q = k;
        int i;
        for (j = 1; j <= k; j++) {
            idx_t s = 2 * (j - 1) + uk - v2f(k - j);
            if (s < 128) sum += (E[j] * A[k - j]) << s;
        }
        while (!(q & 1)) { q >>= 1; t++; }
        o = (u128_t)q; inv = o;
        for (i = 0; i < 7; i++) inv *= (u128_t)2 - o * inv;
        A[k] = (sum >> t) * inv;
    }
    return u128_to_seq(A[n]);
}

/* ---------- arbitrary-precision path, memoized ---------- */
static seq_t *bA = NULL, *bE = NULL, *rA = NULL, *rB = NULL;
static idx_t aHave = 0, aCap = 0, eCap = 0, rCap = 0, rowM = 0;

static void big_euler(idx_t need) {
    idx_t nc, k;
    if (rCap == 0) {
        rCap = 16; rA = alloc_zero(rCap); rB = alloc_zero(rCap);
        rA[0] = 1; rowM = 0;
        eCap = 8; bE = alloc_zero(eCap); bE[0] = 1;
    }
    if (eCap < need + 1) {
        nc = eCap; while (nc < need + 1) nc *= 2;
        bE = grow_zero(bE, eCap, nc); eCap = nc;
    }
    if (rCap < 2 * need + 2) {
        nc = rCap; while (nc < 2 * need + 2) nc *= 2;
        rA = grow_zero(rA, rCap, nc); rB = grow_zero(rB, rCap, nc); rCap = nc;
    }
    while (rowM < 2 * need) {
        seq_t *tmp;
        rB[0] = 0;
        for (k = 1; k <= rowM + 1; k++) rB[k] = rB[k - 1] + rA[rowM + 1 - k];
        tmp = rA; rA = rB; rB = tmp;
        rowM++;
        if (!(rowM & 1)) bE[rowM / 2] = rA[rowM];
    }
}

static seq_t big_a(idx_t n) {
    idx_t k, j;
    if (n < aHave) return bA[n];
    big_euler(n);
    if (aCap < n + 1) {
        idx_t nc = aCap ? aCap : 8;
        while (nc < n + 1) nc *= 2;
        bA = aCap ? grow_zero(bA, aCap, nc) : alloc_zero(nc);
        aCap = nc;
    }
    if (aHave == 0) { bA[0] = 1; aHave = 1; }
    for (k = aHave; k <= n; k++) {
        seq_t sum = 0;
        idx_t uk = v2f(k);
        for (j = 1; j <= k; j++) {
            idx_t s = 2 * (j - 1) + uk - v2f(k - j);
            sum += (bE[j] * bA[k - j]) << s;
        }
        bA[k] = sum / (seq_t)k;
    }
    aHave = n + 1;
    return bA[n];
}

seq_t a(idx_t n) {
    if (seq_is_big()) return big_a(n);
    return fixed_a(n);
}
