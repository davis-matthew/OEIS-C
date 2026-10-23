/*
 * A160660  Numbers that have an "s" in the middle of their names when written
 *          in American English.
 * https://oeis.org/A160660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   _append_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A160660 – numbers whose American English name contains an
   's' at the exact middle position. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/_append_str.h"

/* ------------------------------------------------------------------ */
/* Word tables for number → text conversion */
static const char * const _num_words[20] = {
    "zero",   "one",    "two",    "three", "four",
    "five",   "six",    "seven",  "eight", "nine",
    "ten",    "eleven", "twelve", "thirteen", "fourteen",
    "fifteen","sixteen","seventeen","eighteen","nineteen"
};

static const char * const _tens_words[10] = {
    "",       "",       "twenty", "thirty", "forty",
    "fifty",  "sixty",  "seventy","eighty","ninety"
};

static const char * const _group_words[10] = {
    "",        /* units                                     */
    "thousand",/* 10^3                                      */
    "million", /* 10^6                                      */
    "billion", /* 10^9                                      */
    "trillion",/* 10^12                                     */
    "quadrillion",/* 10^15                               */
    "quintillion",/* 10^18                               */
    "sextillion",/* 10^21                               */
    "septillion",/* 10^24                               */
    "octillion"   /* 10^27                               */
};

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Helper: convert a three‑digit number (0 ≤ v < 1000) to text
   and append it to the destination buffer */
static void _three_to_text(uint64_t v, char *buf, size_t *pos)
{
    if (v >= 100) {
        _append_str(_num_words[v / 100], buf, pos);
        _append_str("hundred", buf, pos);
        v %= 100;
    }
    if (v >= 20) {
        _append_str(_tens_words[v / 10], buf, pos);
        v %= 10;
    }
    if (v > 0) {
        _append_str(_num_words[v], buf, pos);
    }
    /* v == 0 handled by caller (skip the group) */
}

/* ------------------------------------------------------------------ */
/* Build the concatenated American English name of n (n ≥ 1).
   Returns the length of the name in *len and writes into buf */
static size_t _name_len(uint64_t n, char *buf)
{
    /* Decompose n into groups of three digits (units, thousands, …) */
    uint64_t groups[10] = {0};
    int group_cnt = 0;
    uint64_t temp = n;
    while (temp > 0 && group_cnt < 10) {
        groups[group_cnt++] = temp % 1000;
        temp /= 1000;
    }

    size_t pos = 0;
    for (int i = group_cnt - 1; i >= 0; --i) {
        uint64_t g = groups[i];
        if (g == 0) continue;            /* skip empty groups             */
        _three_to_text(g, buf, &pos);
        if (i > 0) _append_str(_group_words[i], buf, &pos);
    }
    return pos;
}

/* ------------------------------------------------------------------ */
/* Predicate: does the American English name of n have an 's'
   exactly in the middle position? */
static bool _has_middle_s(uint64_t n)
{
    char buf[256];          /* ample buffer for names up to 2^64‑1   */
    size_t len = _name_len(n, buf);
    if (len % 2 == 0) return false;          /* even length → no exact middle */

    size_t mid = len / 2;                     /* 0‑based index of middle char   */
    return buf[mid] == 's' || buf[mid] == 'S';
}

/* ------------------------------------------------------------------ */
/* Sequence cache                                                     */
static seq_t * _cache = NULL;      /* store results as seq_t                */
static idx_t  _cache_size = 0;     /* number of elements currently cached  */
static idx_t  _cache_cap   = 0;     /* capacity of _cache array             */
static uint64_t _next_num = 1;     /* first number yet to be examined      */

/* ------------------------------------------------------------------ */
static void _grow_cache(idx_t needed)
{
    if (_cache_cap >= needed) return;
    idx_t new_cap = _cache_cap ? _cache_cap * 2 : 128;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
    if (!tmp) return;                       /* out‑of‑memory handling omitted     */
    _cache = tmp;
    _cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Public sequence function                                           */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, harmless guard */

    /* Ensure we have computed at least the first n terms */
    while (_cache_size < n) {
        if (_has_middle_s(_next_num)) {
            _grow_cache(_cache_size + 1);
            _cache[_cache_size] = (seq_t)_next_num;
            ++_cache_size;
        }
        ++_next_num;
    }
    return _cache[n - 1];
}
