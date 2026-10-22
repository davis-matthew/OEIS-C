/*
 * A001531  Number of self-dual Boolean functions of n variables that are
 *          distinct under complementation/permutation.
 * https://oeis.org/A001531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   Burnside's lemma over the hyperoctahedral group B_m, m = n+1:
 *            a(n) = (1/(2^m m!)) * Sum_{signed cycle types g} |class(g)| *
 *            Fix(g), where Fix(g) = 2^(c(g)/2), c(g) being the number of
 *            cycles of g on {0,1}^m, and Fix(g) = 0 if some g^t *
 *            (complement) has a fixed point, i.e. if there is e >= 0 with...
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   s1531_gcd, s1531_v2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/s1531_gcd.h"
#include "../utils/s1531_v2.h"

static int s1531_P[80], s1531_Q[80];
static int s1531_M;
static uint32_t *s1531_B;
static size_t s1531_BL;

static void s1531_addsh(unsigned __int128 v, uint64_t E)
{
    size_t idx = (size_t)(E / 32);
    unsigned bit = (unsigned)(E % 32);
    unsigned __int128 x = v << bit;
    uint64_t carry = 0;
    while ((x || carry) && idx < s1531_BL) {
        uint64_t s = (uint64_t)s1531_B[idx] + (uint64_t)(uint32_t)x + carry;
        s1531_B[idx] = (uint32_t)s;
        carry = s >> 32;
        x >>= 32;
        idx++;
    }
}

static void s1531_leaf(void)
{
    int m = s1531_M, k;
    unsigned __int128 tot = 1, cent = 1, size, sum = 0;
    int hasneg = 0, e = 0, consistent = 1, zero;
    uint64_t L = 1, t, C;
    for (k = 1; k <= m; k++) tot *= (unsigned __int128)(2 * k);
    for (k = 1; k <= m; k++) {
        int j = s1531_P[k] + s1531_Q[k], i;
        for (i = 0; i < j; i++) cent *= (unsigned __int128)(2 * k);
        for (i = 2; i <= s1531_P[k]; i++) cent *= (unsigned __int128)i;
        for (i = 2; i <= s1531_Q[k]; i++) cent *= (unsigned __int128)i;
    }
    size = tot / cent;
    for (k = 1; k <= m; k++) {
        if (s1531_Q[k] > 0) {
            int v = s1531_v2((uint64_t)k);
            if (!hasneg) { e = v; hasneg = 1; }
            else if (v != e) consistent = 0;
        }
    }
    if (hasneg) {
        zero = consistent;
        if (zero)
            for (k = 1; k <= m; k++)
                if (s1531_P[k] > 0 && s1531_v2((uint64_t)k) <= e) zero = 0;
    } else {
        zero = 1;
        for (k = 1; k <= m; k++)
            if (s1531_P[k] > 0 && (k & 1)) zero = 0;
    }
    if (zero) return;
    for (k = 1; k <= m; k++) {
        if (s1531_P[k] > 0) L = L / s1531_gcd(L, (uint64_t)k) * (uint64_t)k;
        if (s1531_Q[k] > 0) L = L / s1531_gcd(L, (uint64_t)(2 * k)) * (uint64_t)(2 * k);
    }
    for (t = 0; t < L; t++) {
        int bad = 0;
        uint64_t ex = 0;
        for (k = 1; k <= m && !bad; k++) {
            int j = s1531_P[k] + s1531_Q[k];
            uint64_t d;
            if (j == 0) continue;
            d = t ? s1531_gcd((uint64_t)k, t) : (uint64_t)k;
            ex += (uint64_t)j * d;
            if (s1531_Q[k] > 0 && ((t / d) & 1)) bad = 1;
        }
        if (!bad) sum += (unsigned __int128)1 << ex;
    }
    C = (uint64_t)(sum / L);
    s1531_addsh(size, C / 2);
}

static void s1531_rec(int k, int rem)
{
    int j, pp;
    if (rem == 0) { s1531_leaf(); return; }
    if (k > rem) return;
    for (j = 0; j * k <= rem; j++) {
        for (pp = 0; pp <= j; pp++) {
            s1531_P[k] = pp;
            s1531_Q[k] = j - pp;
            s1531_rec(k + 1, rem - j * k);
        }
    }
    s1531_P[k] = 0;
    s1531_Q[k] = 0;
}

seq_t a(idx_t n)
{
    int m = (int)n + 1, k;
    size_t i, ws, top;
    unsigned bs;
    seq_t r = 0;
    s1531_M = m;
    s1531_BL = ((size_t)1 << (m - 1)) / 32 + 8;
    s1531_B = (uint32_t *)calloc(s1531_BL, sizeof(uint32_t));
    memset(s1531_P, 0, sizeof(s1531_P));
    memset(s1531_Q, 0, sizeof(s1531_Q));
    s1531_rec(1, m);
    /* divide by 2^m */
    ws = (size_t)m / 32;
    bs = (unsigned)(m % 32);
    for (i = 0; i < s1531_BL; i++) {
        size_t src = i + ws;
        uint32_t lo = src < s1531_BL ? s1531_B[src] : 0;
        uint32_t hi = src + 1 < s1531_BL ? s1531_B[src + 1] : 0;
        s1531_B[i] = bs ? (uint32_t)((lo >> bs) | (hi << (32 - bs))) : lo;
    }
    /* divide by m! */
    for (k = 2; k <= m; k++) {
        uint64_t rm = 0;
        for (i = s1531_BL; i-- > 0;) {
            uint64_t cur = (rm << 32) | s1531_B[i];
            s1531_B[i] = (uint32_t)(cur / (uint64_t)k);
            rm = cur % (uint64_t)k;
        }
    }
    top = s1531_BL;
    while (top > 0 && s1531_B[top - 1] == 0) top--;
    for (i = top; i-- > 0;) {
        int b;
        for (b = 3; b >= 0; b--) {
            r = (seq_t)(r * 256u);
            r += (seq_t)((s1531_B[i] >> (8 * b)) & 255u);
        }
    }
    free(s1531_B);
    s1531_B = NULL;
    return r;
}
