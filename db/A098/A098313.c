/*
 * A098313  Inverse of A098311.
 * https://oeis.org/A098313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of n in A098311, found by generating A098311
 *            (a(1)=1; a(i) = smallest unused number coprime to a(ceil(i/2)))
 *            until n has appeared, and recording the inverse permutation
 * Range:     u8 n <= 231, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9895 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

static uint64_t *_f = NULL;      /* forward terms of A098311, 1-indexed */
static size_t _fcnt = 0, _fcap = 0;
static uint8_t *_used = NULL;    /* used flags by value */
static uint64_t *_inv = NULL;    /* inverse positions by value */
static size_t _ucap = 0;
static uint64_t _low = 1;        /* smallest possibly unused value */

static uint64_t _A098313_gcd64(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static void _grow_val(size_t need)
{
    if (_ucap >= need) return;
    size_t nc = _ucap ? _ucap * 2 : 64;
    while (nc < need) nc *= 2;
    uint8_t *u = (uint8_t *)realloc(_used, nc);
    if (!u) abort();
    _used = u;
    uint64_t *v = (uint64_t *)realloc(_inv, nc * sizeof(uint64_t));
    if (!v) abort();
    _inv = v;
    memset(_used + _ucap, 0, nc - _ucap);
    memset(_inv + _ucap, 0, (nc - _ucap) * sizeof(uint64_t));
    _ucap = nc;
}

static void _grow_f(size_t need)
{
    if (_fcap >= need) return;
    size_t nc = _fcap ? _fcap * 2 : 64;
    while (nc < need) nc *= 2;
    uint64_t *t = (uint64_t *)realloc(_f, nc * sizeof(uint64_t));
    if (!t) abort();
    _f = t; _fcap = nc;
}

static void _gen_one(void)
{
    if (_fcnt == 0) {
        _grow_f(2); _grow_val(2);
        _f[1] = 1; _used[1] = 1; _inv[1] = 1; _fcnt = 1; _low = 2;
        return;
    }
    size_t i = _fcnt + 1;
    _grow_f(i + 1);
    uint64_t ref = _f[(i + 1) / 2];
    for (;;) {
        _grow_val((size_t)_low + 1);
        if (!_used[_low]) break;
        _low++;
    }
    uint64_t c = _low;
    for (;; c++) {
        _grow_val((size_t)c + 1);
        if (!_used[c] && _A098313_gcd64(c, ref) == 1) break;
    }
    _f[i] = c; _used[c] = 1; _inv[c] = (uint64_t)i; _fcnt = i;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    for (;;) {
        if ((size_t)n < _ucap && _inv[n] != 0) return (seq_t)_inv[n];
        _gen_one();
    }
}
