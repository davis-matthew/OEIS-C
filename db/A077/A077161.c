/*
 * A077161  Final terms of rows of triangle in A077159.
 * https://oeis.org/A077161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = last term of row n of the triangle A077159, where row n
 *            holds the smallest n unused numbers coprime to n (row 1 = {2});
 *            computed by direct simulation of the rows
 * Range:     u8 n <= 21, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static unsigned long long *A077161_last = NULL; /* last term of each row, 1-based */
static size_t A077161_rows = 0;
static size_t A077161_cap = 0;
static unsigned char *A077161_used = NULL;
static size_t A077161_ucap = 0;

static unsigned long long A077161_gcd(unsigned long long x, unsigned long long y)
{
    while (y) { unsigned long long t = x % y; x = y; y = t; }
    return x;
}

static void A077161_ensure_used(size_t n)
{
    if (A077161_ucap <= n) {
        size_t nc = A077161_ucap ? A077161_ucap : 256;
        while (nc <= n) nc <<= 1;
        unsigned char *t = (unsigned char *)realloc(A077161_used, nc);
        if (!t) abort();
        for (size_t i = A077161_ucap; i < nc; ++i) t[i] = 0;
        A077161_used = t;
        A077161_ucap = nc;
    }
}

static void A077161_ensure_last(size_t n)
{
    if (A077161_cap <= n) {
        size_t nc = A077161_cap ? A077161_cap : 64;
        while (nc <= n) nc <<= 1;
        unsigned long long *t = (unsigned long long *)realloc(A077161_last, nc * sizeof(unsigned long long));
        if (!t) abort();
        A077161_last = t;
        A077161_cap = nc;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t need = (size_t)n;
    A077161_ensure_last(need + 1);
    if (A077161_rows == 0) {
        A077161_ensure_used(4);
        A077161_used[2] = 1;
        A077161_last[1] = 2;
        A077161_rows = 1;
    }
    while (A077161_rows < need) {
        unsigned long long r = (unsigned long long)A077161_rows + 1;
        unsigned long long placed = 0, cand = 1, lastv = 0;
        while (placed < r) {
            A077161_ensure_used((size_t)cand);
            if (!A077161_used[cand] && A077161_gcd(cand, r) == 1) {
                A077161_used[cand] = 1;
                lastv = cand;
                ++placed;
            }
            ++cand;
        }
        ++A077161_rows;
        A077161_last[A077161_rows] = lastv;
    }
    return (seq_t)A077161_last[need];
}
