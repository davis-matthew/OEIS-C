/*
 * A140494  Numbers that cannot be part of the trajectory of any number under
 *          repeated application of the map: n -> n + third-smallest number
 *          that does not divide n, unless they are the first term of the
 *          trajectory.
 * https://oeis.org/A140494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   _third_nondivisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A140494: numbers that never appear as a non‑first term
   of the iteration of f(n) = n + t(n), where t(n) is the third
   smallest positive integer that does NOT divide n.
   The algorithm keeps a boolean array of reachable values
   up to a current limit and rebuilds it whenever more
   missing numbers are required. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/_third_nondivisor.h"

static idx_t _reach_limit = 0;        /* size of reachable array */
static bool * _reachable = NULL;       /* array, indices 0.._reach_limit */

/* cache of missing numbers (sequence terms) */
static seq_t * _missing = NULL;        /* array of missing numbers */
static idx_t   _missing_len = 0;       /* number of terms stored */

/* --- auxiliary --------------------------------------------------- */

/* rebuild the reachable array and missing cache up to new_limit. */
static bool _rebuild(idx_t new_limit)
{
    if (new_limit <= _reach_limit) return true; /* nothing to do */

    /* allocate new reachable array */
    bool *reach = (bool *)malloc((new_limit + 1) * sizeof(bool));
    if (!reach) return false;
    /* initialise to false */
    memset(reach, 0, (new_limit + 1) * sizeof(bool));

    /* mark reachable values f(k) = k + t(k) for k = 1..new_limit */
    for (idx_t k = 1; k <= new_limit; ++k)
    {
        idx_t t = _third_nondivisor(k);
        idx_t f = k + t;
        if (f <= new_limit) reach[f] = true;
    }

    /* collect missing numbers */
    idx_t new_len = 0;
    for (idx_t i = 1; i <= new_limit; ++i)
        if (!reach[i]) ++new_len;

    seq_t *m = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!m) { free(reach); return false; }

    idx_t pos = 0;
    for (idx_t i = 1; i <= new_limit; ++i)
        if (!reach[i]) m[pos++] = (seq_t)i;

    /* replace old data */
    if (_reachable) free(_reachable);
    if (_missing)   free(_missing);
    _reachable   = reach;
    _missing     = m;
    _reach_limit = new_limit;
    _missing_len = new_len;

    return true;
}

/* --- required function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but avoid UB */

    /* ensure we have at least n missing numbers */
    while (_missing_len < n)
    {
        idx_t new_limit = _reach_limit ? (_reach_limit * 2) : 16;
        if (new_limit < n * 4) new_limit = n * 4;  /* safety margin */
        if (!_rebuild(new_limit)) return 0;        /* allocation failure */
    }

    return _missing[n - 1];
}
