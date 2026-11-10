/*
 * A075078  Composite numbers which do not arise as a sum of two neighboring
 *          terms of A075077.
 * https://oeis.org/A075078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th (from 0) composite not equal to a(k)+a(k+1) for any
 *            k, where A075077 is built greedily; computed by search
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 56 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static unsigned char *A75_used = NULL;
static size_t A75_used_cap = 0;
static unsigned char *A75_sums = NULL;
static size_t A75_sums_cap = 0;
static uint64_t A75_prev = 1;
static uint64_t A75_min = 1;
static bool A75_init = false;
static uint64_t *A75_found = NULL;
static size_t A75_nfound = 0, A75_fcap = 0;
static uint64_t A75_c = 4;

static void A75_grow(unsigned char **arr, size_t *cap, size_t idx)
{
    if (idx < *cap) return;
    size_t nc = *cap ? *cap : 1024;
    while (nc <= idx) nc <<= 1;
    unsigned char *t = (unsigned char *)realloc(*arr, nc);
    if (!t) abort();
    for (size_t i = *cap; i < nc; ++i) t[i] = 0;
    *arr = t;
    *cap = nc;
}

static bool A75_isprime(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

static void A75_step(void)
{
    if (!A75_init) {
        A75_init = true;
        A75_grow(&A75_used, &A75_used_cap, 1);
        A75_used[1] = 1;
        A75_prev = 1;
        A75_min = 1;
    }
    while (1) {
        A75_grow(&A75_used, &A75_used_cap, (size_t)A75_min);
        if (A75_used[A75_min]) ++A75_min; else break;
    }
    uint64_t cand = A75_min;
    while (1) {
        A75_grow(&A75_used, &A75_used_cap, (size_t)cand);
        if (A75_used[cand]) { ++cand; continue; }
        uint64_t s = A75_prev + cand;
        if (A75_isprime(s)) { ++cand; continue; }
        A75_grow(&A75_sums, &A75_sums_cap, (size_t)s);
        if (A75_sums[s]) { ++cand; continue; }
        A75_used[cand] = 1;
        A75_sums[s] = 1;
        A75_prev = cand;
        break;
    }
    while (1) {
        A75_grow(&A75_used, &A75_used_cap, (size_t)A75_min);
        if (A75_used[A75_min]) ++A75_min; else break;
    }
}

seq_t a(idx_t n)
{
    while (A75_nfound <= (size_t)n) {
        uint64_t c = A75_c;
        if (!A75_init) A75_step();
        while (A75_min <= c) A75_step();
        bool in_sums = (c < A75_sums_cap) && A75_sums[c];
        if (!in_sums && !A75_isprime(c)) {
            if (A75_nfound == A75_fcap) {
                A75_fcap = A75_fcap ? A75_fcap * 2 : 64;
                A75_found = (uint64_t *)realloc(A75_found, A75_fcap * sizeof(uint64_t));
                if (!A75_found) abort();
            }
            A75_found[A75_nfound++] = c;
        }
        ++A75_c;
    }
    return (seq_t)A75_found[(size_t)n];
}
