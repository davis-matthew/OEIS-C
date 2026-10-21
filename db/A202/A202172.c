/*
 * A202172  Squares containing only the digits 0, 4 or 8.
 * https://oeis.org/A202172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A202170(n)^2
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 26, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

static idx_t next_m = 0;

/* Test whether every decimal digit of x is in {0,4,8}.  Works
   for built‑in integer types (C) and for classes that overload the
   usual arithmetic operators (C++). */
static bool square_valid(seq_t x)
{
    if (x == 0) return true;
    while (x)
    {
        seq_t d = x % 10;
        if (d != 0 && d != 4 && d != 8) return false;
        x /= 10;
    }
    return true;
}

/* Cache implementation --------------------------------------------------- */

#ifdef __cplusplus
/* C++: use std::vector for automatic construction/destruction. */
static std::vector<seq_t> cache_vec;
static void ensure_cache(idx_t n)
{
    while (cache_vec.size() < (size_t)n)
    {
        seq_t sq = (seq_t)next_m * (seq_t)next_m;
        if (square_valid(sq)) cache_vec.push_back(sq);
        ++next_m;
    }
}
#define CACHE_AT(n) cache_vec[(size_t)(n)-1]
#else
/* C: use malloc/realloc of plain data. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap << 1 : 128;
    while (newcap < need) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = newcap;
}

static void add_to_cache(seq_t val)
{
    if (cache_len == cache_cap) grow_cache(cache_len + 1);
    cache[cache_len++] = val;
}

static void ensure_cache(idx_t n)
{
    while (cache_len < (size_t)n)
    {
        seq_t sq = (seq_t)next_m * (seq_t)next_m;
        if (square_valid(sq)) add_to_cache(sq);
        ++next_m;
    }
}
#define CACHE_AT(n) cache[(size_t)(n)-1]
#endif

/* ----------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* behaviour for n<1 is irrelevant */
    ensure_cache(n);
    return CACHE_AT(n);
}
