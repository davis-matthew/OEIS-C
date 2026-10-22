/*
 * A000814  Number of switching networks (see Harrison reference for precise
 *          definition).
 * https://oeis.org/A000814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(2^n*n!)) * Sum_{g in B_n} 4^{c(g)}, where B_n is the
 *            hyperoctahedral group acting on {0,1}^n and c(g) is the number
 *            of cycles of g on the 2^n points (Burnside count of pairs of
 *            Boolean functions up to permuting and complementing inputs). Sum
 *            over signed cycle types; c(g) = (1/L) Sum_{t...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   s814_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/s814_gcd.h"

static idx_t s814_n;
static int *s814_P, *s814_Q;
static seq_t s814_T, s814_O;
static int s814_big, s814_W;

static seq_t s814_pow2(unsigned long long e)
{
    if (!s814_big) {
        if (e >= (unsigned long long)s814_W) return (seq_t)0;
        return ((seq_t)1) << (int)e;
    }
    seq_t r = 1, b = 2;
    while (e) {
        if (e & 1) r *= b;
        e >>= 1;
        if (e) b *= b;
    }
    return r;
}

static void s814_visit(void)
{
    idx_t n = s814_n, k;
    unsigned long long L = 1, t;
    for (k = 1; k <= n; k++) {
        if (s814_P[k]) L = L / s814_gcd(L, k) * k;
        if (s814_Q[k]) L = L / s814_gcd(L, 2 * k) * (2 * k);
    }
    unsigned __int128 s = 0;
    for (t = 0; t < L; t++) {
        int ok = 1;
        unsigned long long ex = 0;
        for (k = 1; k <= n && ok; k++) {
            if (s814_P[k]) ex += (unsigned long long)s814_P[k] * s814_gcd(k, t);
            if (s814_Q[k]) {
                unsigned long long g = s814_gcd(k, t);
                if ((t / g) & 1) ok = 0;
                ex += (unsigned long long)s814_Q[k] * g;
            }
        }
        if (ok) s += ((unsigned __int128)1) << ex;
    }
    unsigned long long c = (unsigned long long)(s / L);

    /* centralizer order z = prod a_k! b_k! (2k)^(a_k+b_k) = 2^e * o */
    unsigned long long e = 0;
    seq_t o = 1;
    for (k = 1; k <= n; k++) {
        int cnt[2], j, i;
        cnt[0] = s814_P[k]; cnt[1] = s814_Q[k];
        for (j = 0; j < 2; j++) {
            for (i = 1; i <= cnt[j]; i++) {
                unsigned long long f = (unsigned long long)i;
                while (!(f & 1)) { f >>= 1; e++; }
                o *= (seq_t)f;
                f = 2 * k;
                while (!(f & 1)) { f >>= 1; e++; }
                o *= (seq_t)f;
            }
        }
    }
    s814_T += s814_pow2(2 * c - e) * (s814_O / o);
}

static void s814_rec(idx_t k, idx_t rem)
{
    if (k > s814_n) {
        if (rem == 0) s814_visit();
        return;
    }
    idx_t a, b;
    for (a = 0; a * k <= rem; a++) {
        s814_P[k] = (int)a;
        for (b = 0; (a + b) * k <= rem; b++) {
            s814_Q[k] = (int)b;
            s814_rec(k + 1, rem - (a + b) * k);
        }
    }
    s814_P[k] = 0;
    s814_Q[k] = 0;
}

seq_t a(idx_t n)
{
    idx_t i;
    s814_big = (((seq_t)0 - 1) < (seq_t)0);
    if (!s814_big) {
        seq_t x = 1;
        s814_W = 0;
        while (x != 0 && s814_W < 1024) { x <<= 1; s814_W++; }
    }
    s814_n = n;
    s814_P = (int *)calloc((size_t)n + 2, sizeof(int));
    s814_Q = (int *)calloc((size_t)n + 2, sizeof(int));
    s814_O = 1;
    for (i = 1; i <= n; i++) {
        unsigned long long f = i;
        while (!(f & 1)) f >>= 1;
        s814_O *= (seq_t)f;
    }
    s814_T = 0;
    s814_rec(1, n);
    free(s814_P);
    free(s814_Q);

    seq_t res;
    if (s814_big) {
        res = s814_T / s814_O;
    } else {
        seq_t inv = s814_O;
        int it;
        for (it = 0; it < 7; it++) inv *= (seq_t)2 - s814_O * inv;
        res = s814_T * inv;
    }
    return res;
}
