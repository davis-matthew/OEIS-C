/*
 * A043722  Numbers whose number of runs in the base-2 representation is
 *          congruent to 1 mod 3.
 * https://oeis.org/A043722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 964 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = nth positive integer whose binary representation has
 * a number of runs congruent to 1 modulo 3. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return the number of runs in the binary representation of x modulo 3 */
static int _runs_mod3(seq_t x)
{
    int runs = 0;
    int prev = -1;
    while (x > (seq_t)0) {
        int bit = (int)(x & (seq_t)1);
        if (bit != prev) {
            ++runs;
            prev = bit;
        }
        x >>= 1;
    }
    return runs % 3;
}

/* Static cache for already computed terms */
static seq_t *_cache = NULL;   /* array of terms */
static size_t  _cap   = 0;     /* current capacity of _cache    */
static size_t  _len   = 0;     /* number of terms stored       */
static seq_t   _last_checked = (seq_t)0;   /* last number examined      */

static void _ensure_capacity(size_t needed)
{
    if (needed <= _cap)
        return;
    size_t new_cap = _cap ? _cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = mem_resize(seq_t, _cache, new_cap);
    _cache = tmp;
    _cap   = new_cap;
}

seq_t a(idx_t n)
{
    /* Build the cache up to the required index n */
    while (_len < n) {
        /* Test the next integer after the last examined one */
        _last_checked += (seq_t)1;
        if (_runs_mod3(_last_checked) == 1) {
            _ensure_capacity(_len + 1);
            _cache[_len++] = _last_checked;
        }
    }
    return _cache[n - 1];
}
