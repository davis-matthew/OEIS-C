/*
 * A098311  Smallest number not occurring earlier and coprime to
 *          a(ceiling(n/2)); a(1)=1.
 * https://oeis.org/A098311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(1)=1; a(n) = smallest natural integer not used before
   that is coprime to a(ceil(n/2))  (OEIS A098311) */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/gcd.h"

static seq_t * _a_seq = NULL;      /* sequence table, index 0 unused */
static size_t  _a_cap = 0;         /* largest index stored */

static uint8_t * _used  = NULL;    /* flag array: _used[x]==1 iff x already used */
static size_t   _used_cap = 0;     /* size of _used array (indices 0.._used_cap-1) */

/* enlarge used array to contain indices 0..(min_cap-1) */
static void _ensure_used(size_t min_cap)
{
    if (_used_cap >= min_cap) return;
    size_t new_cap = _used_cap ? _used_cap * 2 : 16;
    while (new_cap < min_cap) new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(_used, new_cap * sizeof(uint8_t));
    if (!tmp) abort();
    memset(tmp + _used_cap, 0, (new_cap - _used_cap) * sizeof(uint8_t));
    _used = tmp;
    _used_cap = new_cap;
}

/* compute a(1) … a(n) if not already present */
static void _ensure_sequence(idx_t n)
{
    if (n <= _a_cap) return;

    /* enlarge sequence table */
    size_t new_cap = (size_t)n + 1;               /* include index n, 0 unused */
    seq_t *tmp = (seq_t *)realloc(_a_seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    _a_seq = tmp;

    /* initialise new entries to 0 */
    for (idx_t i = _a_cap + 1; i <= n; ++i) _a_seq[i] = 0;

    /* make sure used array is at least of size 1 */
    _ensure_used(1);

    /* base term a(1)=1 (only set once) */
    if (_a_cap == 0) {
        _a_seq[1] = (seq_t)1;
        _used[1]  = 1;
    }

    /* compute missing terms */
    idx_t start = (_a_cap > 1) ? _a_cap + 1 : 2;
    for (idx_t i = start; i <= n; ++i) {
        idx_t  mid = (i + 1) / 2;          /* ceil(i/2) */
        seq_t  ref = _a_seq[mid];

        for (size_t cand = 1; ; ++cand) {
            if (cand >= _used_cap) _ensure_used(cand + 1);
            if (!_used[cand]) {
                seq_t cand_val = (seq_t)cand;
                if (gcd(cand_val, ref) == 1) {
                    _a_seq[i] = cand_val;
                    _used[cand] = 1;
                    break;
                }
            }
        }
    }

    _a_cap = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* safety; offset starts at 1 */
    _ensure_sequence(n);
    return _a_seq[n];
}
