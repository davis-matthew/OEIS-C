/*
 * A105830  For n>2, a(n) not appeared previously is such that
 *          a(n-1)^2+4*a(n-2)*a(n) is a minimal square, a(1)=1, a(2)=5.
 * https://oeis.org/A105830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9310 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, a(2) = 5; a(n) is the least c not used yet with
   a(n-1)^2 + 4 a(n-2) c a square. The terms stay small, so this runs in 64
   bits with a bitmap of the values used (in seq_t, searching the whole
   sequence for each candidate, it timed out under big); terms are cached. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static bool is_square_u64(uint64_t x)
{
    uint64_t r = 0;
    for (uint64_t b = (uint64_t)1 << 31; b; b >>= 1)
        if ((r + b) * (r + b) <= x) r += b;
    return r * r == x;
}

seq_t a(idx_t n)
{
    static seq_t *A = NULL;
    static uint64_t *V = NULL;                /* the terms in 64 bits */
    static size_t len = 0, cap = 0;
    static unsigned char *used = NULL;
    static size_t ucap = 0;
    if (n == 0) return (seq_t)0;
    if (!len) {
        cap = 64;
        A = mem_new(seq_t, cap);
        V = (uint64_t *)malloc(cap * sizeof *V);
        ucap = 1024;
        used = (unsigned char *)calloc(ucap, 1);
        if (!A || !V || !used) abort();
        V[0] = 1; V[1] = 5;
        A[0] = (seq_t)1; A[1] = (seq_t)5;
        used[1] = used[5] = 1;
        len = 2;
    }
    while (len < (size_t)n) {
        uint64_t p = V[len - 1], q = V[len - 2], c;
        for (c = 1;; ++c) {
            if (c >= ucap) {
                unsigned char *t = (unsigned char *)realloc(used, 2 * ucap);
                if (!t) abort();
                for (size_t i = ucap; i < 2 * ucap; ++i) t[i] = 0;
                used = t;
                ucap *= 2;
            }
            if (!used[c] && is_square_u64(p * p + 4 * q * c)) break;
        }
        if (len == cap) {
            cap *= 2;
            seq_t *t = mem_resize(seq_t, A, cap);
            uint64_t *v = (uint64_t *)realloc(V, cap * sizeof *v);
            if (!t || !v) abort();
            A = t;
            V = v;
        }
        used[c] = 1;
        V[len] = c;
        A[len++] = (seq_t)c;
    }
    return A[n - 1];
}
