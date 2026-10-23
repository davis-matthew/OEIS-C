/*
 * A042038  Numerators of continued fraction convergents to sqrt(543).
 * https://oeis.org/A042038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numerators of continued fraction convergents to sqrt(543)
   (OEIS A042038)
   Formula: a(n) = 1338674*a(n-10) - a(n-20)  for n ≥ 20
   Initial values:
   a(0)=23, a(1)=70, a(2)=233, a(3)=769, a(4)=1002, a(5)=14797,
   a(6)=15799, a(7)=62194, a(8)=202381, a(9)=669337,
   a(10)=30991883, a(11)=93644986, a(12)=311926841,
   a(13)=1029425509, a(14)=1341352350, a(15)=19808358409,
   a(16)=21149710759, a(17)=83257490686, a(18)=270922182817,
   a(19)=896024039137 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t _init[20] = {
    (seq_t)23ULL,
    (seq_t)70ULL,
    (seq_t)233ULL,
    (seq_t)769ULL,
    (seq_t)1002ULL,
    (seq_t)14797ULL,
    (seq_t)15799ULL,
    (seq_t)62194ULL,
    (seq_t)202381ULL,
    (seq_t)669337ULL,
    (seq_t)30991883ULL,
    (seq_t)93644986ULL,
    (seq_t)311926841ULL,
    (seq_t)1029425509ULL,
    (seq_t)1341352350ULL,
    (seq_t)19808358409ULL,
    (seq_t)21149710759ULL,
    (seq_t)83257490686ULL,
    (seq_t)270922182817ULL,
    (seq_t)896024039137ULL
};

/* Cache for computed terms */
static seq_t * _cache = NULL;   /* allocated buffer */
static size_t  _cache_cap = 0;  /* allocated capacity in elements */
static size_t  _cache_len = 0;  /* number of elements stored (indices 0.._cache_len-1) */

static void _ensure_cache(idx_t n)
{
    if (n < 20) return;                 /* nothing to compute */

    size_t need = (size_t)n + 1;        /* we need indices 0..n inclusive */
    if (_cache_len >= need) return;     /* already computed */

    /* Allocate or grow the buffer to the required size */
    if (_cache_cap < need) {
        size_t new_cap = _cache_cap ? _cache_cap : 20;
        while (new_cap < need) new_cap <<= 1;   /* double until big enough */
        seq_t *tmp = (seq_t*)realloc(_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* fatal allocation failure */
        _cache = tmp;
        _cache_cap = new_cap;
    }

    /* Initialise first 20 values if this is the first build */
    if (_cache_len == 0) {
        for (size_t i = 0; i < 20; ++i) _cache[i] = _init[i];
        _cache_len = 20;
    }

    /* Compute remaining terms using the recurrence */
    const seq_t coeff = (seq_t)1338674ULL;
    for (size_t i = _cache_len; i < need; ++i) {
        _cache[i] = coeff * _cache[i-10] - _cache[i-20];
    }

    _cache_len = need;
}

seq_t a(idx_t n)
{
    if (n < 20) return _init[(size_t)n];
    _ensure_cache(n);
    return _cache[(size_t)n];
}
