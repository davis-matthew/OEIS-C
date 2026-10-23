/*
 * A164351  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A164351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence):
   a(0)=1, a(1)=50, a(2)=2450, a(3)=120050, a(4)=5882450, a(5)=288240050
   a(6)=48·(a5+a4+a3+a2+a1) – 1175
   a(n)=48·(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)) – 1176·a(n-6)   for n≥7
*/
#include <stdlib.h>

static seq_t *_seq_cache = NULL;      /* cache holding computed values          */
static size_t _seq_len   = 0;         /* number of elements stored             */
static size_t _seq_cap   = 0;         /* current capacity of the cache          */

/* Grow cache to at least new_cap elements */
static void _grow_cache(size_t new_cap) {
    size_t cap = _seq_cap ? _seq_cap : 16;
    while (cap < new_cap) cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(_seq_cache, cap * sizeof(seq_t));
    if (!new_cache) abort();          /* allocation failure – abort */
    _seq_cache = new_cache;
    _seq_cap   = cap;
}

seq_t a(idx_t n) {
    /* initial constants for the first six terms */
    static const seq_t init[6] = {
        (seq_t)1ULL,
        (seq_t)50ULL,
        (seq_t)2450ULL,
        (seq_t)120050ULL,
        (seq_t)5882450ULL,
        (seq_t)288240050ULL
    };

    /* ensure cache exists and has room for n+1 elements */
    if (_seq_cap < (size_t)n + 1) _grow_cache((size_t)n + 1);

    /* populate missing initial terms if necessary */
    if (_seq_len < (size_t)n + 1) {
        for (size_t i = _seq_len; i < 6 && i <= (size_t)n; ++i)
            _seq_cache[i] = init[i];
        _seq_len = 6 > (size_t)n + 1 ? (size_t)n + 1 : 6;
    }

    /* compute terms up to n */
    for (size_t i = _seq_len; i <= (size_t)n; ++i) {
        if (i < 6) {
            /* already set from init */
        } else {
            seq_t sum = _seq_cache[i-1];
            sum += _seq_cache[i-2];
            sum += _seq_cache[i-3];
            sum += _seq_cache[i-4];
            sum += _seq_cache[i-5];

            if (i == 6) {  /* special constant for a(6) */
                _seq_cache[i] = (seq_t)48 * sum - (seq_t)1175;
            } else {
                _seq_cache[i] = (seq_t)48 * sum - (seq_t)1176 * _seq_cache[i-6];
            }
        }
    }
    _seq_len = (size_t)n + 1;
    return _seq_cache[n];
}
