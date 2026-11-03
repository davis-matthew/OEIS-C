/*
 * A109029  Numbers that have exactly nine prime factors counted with
 *          multiplicity (A046312) whose digit reversal is different and also
 *          has 9 prime factors (with multiplicity).
 * https://oeis.org/A109029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 47 terms through n = 9840 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A109029: numbers with exactly nine prime factors (with multiplicity) whose
 * digit reversal is different and also has nine. Omega comes from a sieve up
 * to a power of 10 (which also holds every reversal of a smaller number); the
 * limit grows tenfold until there are n terms, and the terms are cached (the
 * original factored every integer on every call). */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *terms = NULL;
static size_t term_len = 0;

static void build(uint64_t L)                 /* L a power of 10 */
{
    unsigned char *om = (unsigned char *)calloc((size_t)L, 1);
    uint32_t *rest = (uint32_t *)malloc((size_t)L * sizeof *rest);
    if (!om || !rest) abort();
    for (uint64_t i = 0; i < L; ++i) rest[i] = (uint32_t)i;
    for (uint64_t p = 2; p < L; ++p) {
        if (rest[p] != p || om[p]) continue;      /* p prime: untouched so far */
        for (uint64_t m = p; m < L; m += p)
            while (rest[m] % p == 0) { rest[m] /= (uint32_t)p; ++om[m]; }
    }
    free(rest);
    size_t cnt = 0, cap = 1024;
    uint64_t *t = (uint64_t *)malloc(cap * sizeof *t);
    if (!t) abort();
    for (uint64_t k = 2; k < L; ++k) {
        if (om[k] != 9) continue;
        uint64_t r = 0;
        for (uint64_t x = k; x; x /= 10) r = r * 10 + x % 10;
        if (r == k || om[r] != 9) continue;
        if (cnt == cap) {
            uint64_t *u = (uint64_t *)realloc(t, (cap *= 2) * sizeof *t);
            if (!u) abort();
            t = u;
        }
        t[cnt++] = k;
    }
    free(om);
    free(terms);
    terms = t;
    term_len = cnt;
}

seq_t a(idx_t n)
{
    static uint64_t L = 0;
    if (n < 1) return (seq_t)0;
    while (term_len < (size_t)n) {
        L = L ? L * 10 : 1000000;
        if (L > UINT64_C(1000000000)) return (seq_t)0;
        build(L);
    }
    return (seq_t)terms[n - 1];
}
