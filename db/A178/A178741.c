/*
 * A178741  a(1)=2. For a(n)+1, differences must be strictly increasing and
 *          consecutive terms are relatively prime.
 * https://oeis.org/A178741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2; for n>1, a(n) = a(n-1) + d, where d > a(n-1)-a(n-2) *
 *            and gcd(a(n-1), a(n))=1 (minimal such a(n)). The differences d
 *            are * strictly increasing and consecutive terms are relatively
 *            prime
 * Range:     u8 n <= 19, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Dynamic cache for the sequence */
static seq_t *_seq     = NULL;   /* array of computed terms           */
static size_t   _len    = 0;      /* number of terms stored            */
static size_t   _cap    = 0;      /* current capacity of _seq          */
static seq_t   _prev_diff = 0;    /* difference between the last two terms */

/* Ensure the cache can hold at least _min_cap elements. */
static void _ensure_capacity(size_t _min_cap)
{
    if (_cap >= _min_cap)
        return;

    size_t new_cap = _cap ? _cap * 2 : 16;
    while (new_cap < _min_cap)
        new_cap <<= 1;

    /* cast the void* return of realloc to seq_t* (needed for C++) */
    seq_t *tmp = (seq_t *)realloc(_seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* on allocation failure */
    _seq   = tmp;
    _cap   = new_cap;
}

/* Compute the n-th term of OEIS A178741.
 * n is 1‑based: a(1) is the first term. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n = 0 – safety value */

    /* Convert to size_t safely; the sequence length will never exceed SIZE_MAX */
    size_t target = (size_t)n;

    /* Initialise the first term on first use */
    if (_len == 0) {
        _ensure_capacity(1);
        _seq[0] = 2;            /* a(1) = 2 */
        _len = 1;
        _prev_diff = 0;         /* no previous difference yet */
    }

    /* Build the cache up to the requested index */
    while (_len < target) {
        _ensure_capacity(_len + 1);

        seq_t prev = _seq[_len - 1];
        seq_t step = _prev_diff + 1;   /* strictly larger than previous diff */
        seq_t cand = prev + step;

        /* Find the first candidate whose gcd with prev is 1 */
        while (gcd(prev, cand) != 1) {
            step++;
            cand++;
        }

        _seq[_len] = cand;
        _prev_diff = step;
        _len++;
    }

    return _seq[n - 1];
}
