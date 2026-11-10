/*
 * A082825  Product of terms in row n of A082822.
 * https://oeis.org/A082825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the n entries of row n of A082822 (rows built
 *            greedily by simulation: row r takes the r-1 smallest unused
 *            integers >1 coprime to r, then the smallest unused number
 *            congruent to the inverse of their product mod r)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

static unsigned char *A082825_used = NULL;
static size_t A082825_used_cap = 0;
static seq_t *A082825_prods = NULL;
static size_t A082825_cap = 0;
static idx_t A082825_rows = 0;

static void A082825_ensure_used(uint64_t v)
{
    if (v < A082825_used_cap) return;
    size_t nc = A082825_used_cap ? A082825_used_cap : 64;
    while (nc <= v) nc <<= 1;
    unsigned char *t = (unsigned char *)realloc(A082825_used, nc);
    if (!t) exit(EXIT_FAILURE);
    memset(t + A082825_used_cap, 0, nc - A082825_used_cap);
    A082825_used = t;
    A082825_used_cap = nc;
}

static uint64_t A082825_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static uint64_t A082825_inv(uint64_t a, uint64_t mod)
{
    int64_t t = 0, nt = 1, r = (int64_t)mod, nr = (int64_t)(a % mod);
    while (nr != 0) {
        int64_t q = r / nr, tmp;
        tmp = t - q * nt; t = nt; nt = tmp;
        tmp = r - q * nr; r = nr; nr = tmp;
    }
    if (t < 0) t += (int64_t)mod;
    return (uint64_t)t;
}

static void A082825_row(uint64_t r)
{
    uint64_t m = 2, pm = 1;
    seq_t prod = 1;
    uint64_t got = 0;
    while (got < r - 1) {
        A082825_ensure_used(m);
        if (A082825_used[m] || A082825_gcd(m, r) != 1) { ++m; continue; }
        A082825_used[m] = 1;
        pm = (pm * (m % r)) % r;
        prod = prod * (seq_t)m;
        ++got;
        ++m;
    }
    uint64_t c = A082825_inv(pm, r);
    if (c <= 1) c += r * ((2 - c + r - 1) / r);
    for (;;) {
        A082825_ensure_used(c);
        if (!A082825_used[c] && A082825_gcd(c, r) == 1) break;
        c += r;
    }
    A082825_used[c] = 1;
    prod = prod * (seq_t)c;

    if (r + 1 > A082825_cap) {
        size_t nc = A082825_cap ? A082825_cap * 2 : 64;
        while (nc < r + 1) nc <<= 1;
        seq_t *t = mem_resize(seq_t, A082825_prods, nc);
        if (!t) exit(EXIT_FAILURE);
        A082825_prods = t;
        A082825_cap = nc;
    }
    A082825_prods[r] = prod;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (A082825_rows == 0) {
        A082825_ensure_used(64);
        A082825_used[1] = 1;
        A082825_cap = 64;
        A082825_prods = mem_new(seq_t, 64);
        if (!A082825_prods) exit(EXIT_FAILURE);
        A082825_prods[1] = 1;
        A082825_rows = 1;
    }
    while (A082825_rows < n) {
        A082825_row(A082825_rows + 1);
        ++A082825_rows;
    }
    return A082825_prods[n];
}
