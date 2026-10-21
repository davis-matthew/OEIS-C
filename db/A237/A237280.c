/*
 * A237280  Numbers k such that A000041(k) == 2 (mod 4).
 * https://oeis.org/A237280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _ensure_int_cap, _ensure_seq_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A237280 – numbers k such that the partition number p(k)
   satisfies p(k) ≡ 2 (mod 4).
   The function builds the list of such k incrementally, caching all
   values it has found.  It uses the pentagonal‑number recurrence to
   compute p(k) modulo 4.  Complexity is O(m √m) where m is the
   largest k examined (≤ a(n)). */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/_ensure_int_cap.h"
#include "../utils/_ensure_seq_cap.h"

/* -----  Static cache  ----- */
static idx_t      _last_k      = 0;  /* largest k processed            */
static idx_t      _found_cnt   = 0;  /* number of qualifying k's found */
static size_t     _klist_cap   = 0;  /* capacity of _klist              */
static seq_t     *_klist       = NULL; /* list of qualifying k's          */
static size_t     _pmod4_cap   = 0;  /* capacity of _pmod4arr          */
static int       *_pmod4arr    = NULL; /* p(k) mod 4 for 0 … current k   */

/* -----  Compute next p(k) modulo 4  ----- */
static void _compute_next_pmod4(void)
{
    idx_t k   = _last_k + 1;
    int   sum = 0;

    for (int j = 1; ; ++j) {
        /* Positive generalized pentagonal number */
        idx_t g1 = (idx_t)j * (3ull * (idx_t)j - 1) / 2;
        if (g1 > k) break;
        int sign = (j & 1) ? 1 : -1;
        sum += sign * _pmod4arr[k - g1];

        /* Negative generalized pentagonal number */
        idx_t g2 = (idx_t)j * (3ull * (idx_t)j + 1) / 2;
        if (g2 > k) continue;          /* All later g2 will also be > k */
        sum += sign * _pmod4arr[k - g2];
    }

    /* Reduce modulo 4 (0…3) */
    sum %= 4;
    if (sum < 0) sum += 4;

    _ensure_int_cap(&_pmod4_cap, k + 1, &_pmod4arr);
    _pmod4arr[k] = sum;
    _last_k = k;
}

/* -----  Main sequence function  ----- */
seq_t a(idx_t n)
{
    /* --- lazy initialization of cache --- */
    if (_last_k == 0) {
        _ensure_int_cap(&_pmod4_cap, 1, &_pmod4arr);
        _pmod4arr[0] = 1;                      /* p(0) = 1 */
        _ensure_seq_cap(&_klist_cap, 1, &_klist);
        _found_cnt = 0;
    }

    /* --- extend cache until we have at least n qualifying values --- */
    while (_found_cnt < n) {
        _compute_next_pmod4();                  /* compute p(k) mod 4  */
        if (_pmod4arr[_last_k] == 2) {          /* qualifies?          */
            _ensure_seq_cap(&_klist_cap, _found_cnt + 1, &_klist);
            _klist[_found_cnt] = (seq_t)_last_k;
            ++_found_cnt;
        }
    }

    /* --- return the requested element (1‑based offset) --- */
    return _klist[n - 1];
}
