/*
 * A102859  Numbers that when squared and written backwards give a square
 *          again.
 * https://oeis.org/A102859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 285, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 8637 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A102859: m >= 0 such that m^2 written backwards is a square. m is scanned in
 * order in 64 bits (the terms reach 1e9 by n = 10000, so m^2 < 1e19): the
 * square is updated incrementally, reversed four digits at a time from a
 * table, and tested with a floating-point root corrected exactly. Terms are
 * cached and the scan resumes for larger n. (The original was too slow and
 * wrong from a(3774) on.) */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static uint64_t *terms = NULL;
static size_t term_len = 0, term_cap = 0;
static uint64_t next_m = 0;
static uint16_t rev4[10000];

static uint64_t reverse(uint64_t x)
{
    uint64_t r = 0;
    while (x >= 10000) {
        r = r * 10000 + rev4[x % 10000];
        x /= 10000;
    }
    while (x) {
        r = r * 10 + x % 10;
        x /= 10;
    }
    return r;
}

static int is_square(uint64_t v)
{
    uint64_t s = (uint64_t)sqrtl((long double)v);
    while (s * s > v) --s;
    while ((s + 1) * (s + 1) <= v) ++s;
    return s * s == v;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (!term_cap)
        for (unsigned c = 0; c < 10000; ++c)
            rev4[c] = (uint16_t)((c % 10) * 1000 + (c / 10 % 10) * 100 + (c / 100 % 10) * 10 + c / 1000);
    uint64_t sq = next_m * next_m;
    while (term_len < (size_t)n && next_m < UINT64_C(4000000000)) {
        if (is_square(reverse(sq))) {
            if (term_len == term_cap) {
                term_cap = term_cap ? 2 * term_cap : 1024;
                uint64_t *t = (uint64_t *)realloc(terms, term_cap * sizeof *t);
                if (!t) abort();
                terms = t;
            }
            terms[term_len++] = next_m;
        }
        sq += 2 * next_m + 1;
        ++next_m;
    }
    return (size_t)n <= term_len ? (seq_t)terms[n - 1] : (seq_t)0;
}
