/*
 * A101802  Primes which remain prime after operation "omit leftmost maximal
 *          digit" repeated until only one digit remains.
 * https://oeis.org/A101802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 169, u32 n <= 878, u64 n <= 1250,
 *            u128 n <= 1250, big any n
 * Validated: full, pass, 1250 terms through n = 1250 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A101802: primes that stay prime as their leftmost largest digit is removed,
 * again and again, down to one digit. Built backwards from 2, 3, 5, 7: a child
 * inserts a digit d no smaller than any digit present, to the left of every
 * digit equal to d (so it is the leftmost largest), and must be prime (exact
 * 64-bit test). Every term arises this way exactly once; the tree is searched
 * completely below 2^64, and the terms are sorted and cached. (The original
 * tested integers in turn and ran out of time long before the larger terms.) */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

typedef unsigned __int128 u128;

static uint64_t *terms = NULL;
static size_t term_len = 0, term_cap = 0;

static void push(uint64_t v)
{
    if (term_len == term_cap) {
        term_cap = term_cap ? 2 * term_cap : 1024;
        uint64_t *t = (uint64_t *)realloc(terms, term_cap * sizeof *t);
        if (!t) abort();
        terms = t;
    }
    terms[term_len++] = v;
}

static void grow(uint64_t v)
{
    unsigned char dig[24];
    int len = 0, mx = 0;
    for (uint64_t x = v; x; x /= 10) dig[len++] = (unsigned char)(x % 10);   /* low first */
    for (int i = 0; i < len; ++i) if (dig[i] > mx) mx = dig[i];
    for (int d = mx; d <= 9; ++d) {
        /* positions from the top: insert before the first digit equal to d, or anywhere if none */
        int first = len;                     /* index from the top of the first digit == d */
        for (int i = len - 1; i >= 0; --i) if (dig[i] == d) { first = len - 1 - i; break; }
        for (int p = 0; p <= first; ++p) {
            /* digits above position p, then d, then the rest */
            u128 hi = 0, lo = 0, pw = 1;
            for (int i = len - 1; i >= len - p; --i) hi = hi * 10 + dig[i];
            for (int i = 0; i < len - p; ++i) { lo += (u128)dig[i] * pw; pw *= 10; }
            u128 w = (hi * 10 + (u128)d) * pw + lo;
            if (w >> 64) continue;
            uint64_t c = (uint64_t)w;
            if (is_prime_u64(c)) { push(c); grow(c); }
        }
    }
}

static int cmp_u64(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return (a > b) - (a < b);
}

seq_t a(idx_t n)
{
    if (!terms) {
        static const uint64_t start[4] = {2, 3, 5, 7};
        for (int i = 0; i < 4; ++i) grow(start[i]);
        qsort(terms, term_len, sizeof *terms, cmp_u64);
    }
    if (n < 1 || (size_t)n > term_len) return (seq_t)0;
    return (seq_t)terms[n - 1];
}
