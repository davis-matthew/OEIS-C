/*
 * A005307  Bosonic string states.
 * https://oeis.org/A005307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G.f.: (1-x) * Product_{k>=2} 1/(1-x^k)^(floor(k/2)-1). Computed
 *            with the Euler transform recurrence: n*a(n) = Sum_{k=1..n}
 *            c(k)*a(n-k), with c(k) = Sum_{d|k} d*b(d), where b(1) = -1 and
 *            b(d) = floor(d/2)-1 for d >= 2. a(0) = 1. Fixed-width builds
 *            instead multiply out the product exactly mod 2^128
 * Range:     i8 n <= 16, i16 n <= 16, i32 n <= 16, i64 n <= 16, i128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

static seq_t from_i128(__int128 v)
{
    if (v >= -(__int128)4000000000000000000LL && v <= (__int128)4000000000000000000LL)
        return (seq_t)(long long)v;
    {
        __int128 q = v / ((__int128)1 << 32);
        long long rem = (long long)(v - q * ((__int128)1 << 32));
        return from_i128(q) * (seq_t)4294967296LL + (seq_t)rem;
    }
}

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

#define FIXED_LIM 1000

static seq_t *F = NULL;
static idx_t Flen = 0;
static long long *C = NULL;

static seq_t a_fixed(idx_t n)
{
    static unsigned __int128 *G = NULL;
    if (n > FIXED_LIM) return 0; /* far beyond the range of any fixed-width type */
    if (!G) {
        idx_t L = FIXED_LIM + 1, k, j;
        long long r, e;
        G = (unsigned __int128 *)malloc(L * sizeof(unsigned __int128));
        memset(G, 0, L * sizeof(unsigned __int128));
        G[0] = 1;
        G[1] = ~(unsigned __int128)0; /* -1 mod 2^128 */
        for (k = 4; k < L; k++) {
            e = (long long)(k / 2) - 1;
            for (r = 0; r < e; r++)
                for (j = k; j < L; j++)
                    G[j] += G[j - k];
        }
    }
    return from_i128((__int128)G[n]);
}

seq_t a(idx_t n)
{
    if (seq_is_fixed_width()) return a_fixed(n);

    if (n >= Flen) {
        idx_t newlen = n + 1, d, m, k, start;
        F = grow_zero(F, Flen, newlen);
        C = (long long *)realloc(C, newlen * sizeof(long long));
        for (k = 0; k < newlen; k++) C[k] = 0;
        for (d = 1; d < newlen; d++) {
            long long bd = (d == 1) ? -1 : (long long)(d / 2) - 1;
            if (bd == 0) continue;
            for (m = d; m < newlen; m += d)
                C[m] += (long long)d * bd;
        }
        if (Flen == 0) {
            F[0] = 1;
            start = 1;
        } else {
            start = Flen;
        }
        for (m = start; m < newlen; m++) {
            seq_t s = 0;
            for (k = 1; k <= m; k++) {
                if (C[k] == 0) continue;
                s += F[m - k] * (seq_t)C[k];
            }
            F[m] = s / (seq_t)m;
        }
        Flen = newlen;
    }
    return F[n];
}
