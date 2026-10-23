/*
 * A131091  Partial sums of A131707.
 * https://oeis.org/A131091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partial sums of the partition function modulo 10.  
   Sequence a(n) = Sum_{i=0..n} A001333(i) mod 10.  
   Recurrence used for n>7:
       a(n) = 2*a(n‑1) - a(n‑2) - a(n‑6) + 2*a(n‑7) - a(n‑8)
   with initial values:
       a(0)=1, a(1)=2, a(2)=5, a(3)=12, a(4)=19, a(5)=20, a(6)=29, a(7)=38 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t * _cache = NULL;          /* array of computed a(n) values */
static idx_t   _cache_capacity = 0;    /* capacity of _cache (count of slots) */
static idx_t   _last_filled = 0;       /* highest index that has been computed */
static bool    _initialized = false;   /* true after the first base initialization */

#if defined(__cplusplus)
#define RELOC(ptr, sz) reinterpret_cast<seq_t*>(realloc(ptr, sz))
#else
#define RELOC(ptr, sz) ((seq_t*)realloc(ptr, sz))
#endif

seq_t a(idx_t n)
{
    /* Ensure we have enough capacity for index n and for the first 8 terms */
    idx_t required_cap = (n > 7) ? n + 1 : 8;
    if (_cache_capacity < required_cap) {
        seq_t *new_mem = RELOC(_cache, required_cap * sizeof(seq_t));
        if (!new_mem) abort();
        _cache = new_mem;
        _cache_capacity = required_cap;
    }

    /* Initial base values (only once) */
    if (!_initialized) {
        _cache[0] = (seq_t)1;
        _cache[1] = (seq_t)2;
        _cache[2] = (seq_t)5;
        _cache[3] = (seq_t)12;
        _cache[4] = (seq_t)19;
        _cache[5] = (seq_t)20;
        _cache[6] = (seq_t)29;
        _cache[7] = (seq_t)38;
        _last_filled = 7;
        _initialized = true;
    }

    /* Compute missing terms in order up to n */
    if (n > _last_filled) {
        for (idx_t i = _last_filled + 1; i <= n; ++i) {
            seq_t pos = _cache[i-1];
            pos += _cache[i-1];            /* 2*a(i‑1) */
            pos += _cache[i-7];
            pos += _cache[i-7];            /* +2*a(i‑7) */
            seq_t neg = _cache[i-2];
            neg += _cache[i-6];
            neg += _cache[i-8];            /* -(a(i‑2)+a(i‑6)+a(i‑8)) */
            _cache[i] = pos - neg;         /* pos ≥ neg for all n */
        }
        _last_filled = n;
    }

    return _cache[n];
}
