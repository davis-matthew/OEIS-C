/*
 * A000370  Number of NPN-equivalence classes of Boolean functions of n or
 *          fewer variables.
 * https://oeis.org/A000370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (1/(2^(n+1)*n!)) * Sum_{g in B_n} 2^{c(g)} * (1 + [all
 *            cycles of g on {0,1}^n have even length]), Burnside's lemma for
 *            the hyperoctahedral group B_n (permuting and complementing
 *            inputs) combined with output complementation; c(g) = number of
 *            cycles of g acting on {0,1}^n. The sum runs over...
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   u_gcd, u_phi, v2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/u_gcd.h"
#include "../utils/u_phi.h"
#include "../utils/v2.h"

static int g_n;
static int P[64], Q[64];
static uint32_t *BIG;
static size_t BL;
static unsigned __int128 FACT2;

/* log2 of number of points of {0,1}^n fixed by g^e, or -1 if none */
static long long fexp(uint64_t e)
{
    long long E = 0;
    int k;
    int ve = v2(e);
    for (k = 1; k <= g_n; k++) {
        if (P[k]) E += (long long)P[k] * (long long)u_gcd(e, (uint64_t)k);
        if (Q[k]) {
            if (ve <= v2((uint64_t)k)) return -1;
            E += (long long)Q[k] * (long long)u_gcd(e, (uint64_t)k);
        }
    }
    return E;
}

static void big_add_at(uint32_t v, uint64_t bit)
{
    size_t i;
    uint64_t carry;
    if (!v) return;
    i = (size_t)(bit / 32);
    carry = (uint64_t)v << (bit % 32);
    while (carry && i < BL) {
        uint64_t s = (uint64_t)BIG[i] + (carry & 0xffffffffULL);
        BIG[i] = (uint32_t)s;
        carry = (carry >> 32) + (s >> 32);
        i++;
    }
}

static void process(void)
{
    uint64_t ord = 1, e, o;
    unsigned __int128 S = 0, M;
    uint64_t C;
    int k, i, even, j;
    for (k = 1; k <= g_n; k++) {
        if (P[k]) ord = ord / u_gcd(ord, (uint64_t)k) * (uint64_t)k;
        if (Q[k]) ord = ord / u_gcd(ord, (uint64_t)(2 * k)) * (uint64_t)(2 * k);
    }
    for (e = 1; e <= ord; e++) {
        if (ord % e == 0) {
            long long E = fexp(e);
            if (E >= 0)
                S += (unsigned __int128)u_phi(ord / e) << E;
        }
    }
    C = (uint64_t)(S / ord);
    o = ord;
    while (!(o & 1)) o >>= 1;
    even = (fexp(o) < 0);

    M = FACT2;
    for (k = 1; k <= g_n; k++) {
        for (i = 2; i <= P[k]; i++) M /= (unsigned)i;
        for (i = 2; i <= Q[k]; i++) M /= (unsigned)i;
        for (i = 0; i < P[k] + Q[k]; i++) M /= (unsigned)(2 * k);
    }
    if (even) M *= 2;
    for (j = 0; j < 4; j++) {
        big_add_at((uint32_t)(M & 0xffffffffu), C + 32 * (uint64_t)j);
        M >>= 32;
    }
}

static void rec(int k, int r)
{
    int t, p;
    if (r == 0) { process(); return; }
    if (k > r) return;
    for (t = 0; t * k <= r; t++) {
        for (p = 0; p <= t; p++) {
            P[k] = p;
            Q[k] = t - p;
            rec(k + 1, r - t * k);
        }
    }
    P[k] = 0;
    Q[k] = 0;
}

seq_t a(idx_t n)
{
    int nn = (int)n, i, s;
    size_t top;
    uint64_t rem;
    seq_t r = 0;
    g_n = nn;
    memset(P, 0, sizeof(P));
    memset(Q, 0, sizeof(Q));
    BL = (size_t)((((uint64_t)1 << nn) + 10 * (uint64_t)nn + 64) / 32 + 8);
    BIG = (uint32_t *)calloc(BL, sizeof(uint32_t));
    FACT2 = 1;
    for (i = 1; i <= nn; i++) FACT2 *= (unsigned)(2 * i);

    rec(1, nn);

    /* divide by 2^(n+1) */
    s = nn + 1;
    {
        size_t t;
        for (t = 0; t < BL; t++) {
            uint32_t hi = (t + 1 < BL) ? BIG[t + 1] : 0;
            BIG[t] = (BIG[t] >> s) | (uint32_t)(((uint64_t)hi << (32 - s)) & 0xffffffffULL);
        }
    }
    /* divide by n! */
    for (i = 2; i <= nn; i++) {
        size_t t = BL;
        rem = 0;
        while (t > 0) {
            uint64_t cur;
            t--;
            cur = (rem << 32) | BIG[t];
            BIG[t] = (uint32_t)(cur / (uint64_t)i);
            rem = cur % (uint64_t)i;
        }
    }
    top = BL;
    while (top > 0 && BIG[top - 1] == 0) top--;
    while (top > 0) {
        top--;
        r = r * (seq_t)65536;
        r = r * (seq_t)65536;
        r += (seq_t)BIG[top];
    }
    free(BIG);
    BIG = NULL;
    return r;
}
