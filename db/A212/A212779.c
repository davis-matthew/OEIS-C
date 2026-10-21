/*
 * A212779  Half the number of 0..5 arrays of length n+2 with second
 *          differences nonzero.
 * https://oeis.org/A212779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = 3*a(n‑1) + 12*a(n‑2)
 *            + 12*a(n‑3) – 6*a(n‑4) – 18*a(n‑5) – 14*a(n‑6) – 11*a(n‑7) –
 *            16*a(n‑8) – 11*a(n‑9) – a(n‑10). Initial values (offset 1):
 *            a(1)=99, a(2)=546, a(3)=3010, a(4)=16594, a(5)=91482,
 *            a(6)=504337, a(7)=2780392, a(8)=15328203, a(9)=84503842,...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static const seq_t _BASE[11] = {
    0ULL,          /* unused 0 entry */
    99ULL,          /* a(1) */
    546ULL,         /* a(2) */
    3010ULL,        /* a(3) */
    16594ULL,       /* a(4) */
    91482ULL,       /* a(5) */
    504337ULL,      /* a(6) */
    2780392ULL,     /* a(7) */
    15328203ULL,    /* a(8) */
    84503842ULL,    /* a(9) */
    465866699ULL    /* a(10) */
};

static const int _COEFF[10] = { 3, 12, 12, -6, -18, -14, -11, -16, -11, -1 };

/* Memoized cache of previously computed terms */
static seq_t * _CACHE = NULL;
static size_t  _CACHE_CAP = 0;
static idx_t   _CACHE_LEN = 0;  /* largest n already stored (>=10 once init'd) */

/* Return a(n) for the sequence A212779.  The function is thread‑safe only
   if called from a single thread; no external synchronization is
   required.  The cache grows lazily, so repeated calls with arbitrarily
   ordered n are handled correctly. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Initialise cache lazily on first call */
    if (_CACHE == NULL) {
        /* Allocate space for indices 0..10 (index 0 unused) */
        size_t init_cap = 16;
        _CACHE = (seq_t *)calloc(init_cap, sizeof(seq_t));
        if (_CACHE == NULL) return (seq_t)0; /* allocation failure */
        for (idx_t i = 1; i <= 10; ++i) _CACHE[i] = _BASE[i];
        _CACHE_LEN = 10;
        _CACHE_CAP = init_cap;
    }

    /* Return cached value if already computed */
    if (n <= _CACHE_LEN) return _CACHE[n];

    /* Grow cache as needed */
    if (n >= _CACHE_CAP) {
        size_t new_cap = _CACHE_CAP;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(_CACHE, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failure */
        _CACHE = tmp;
        _CACHE_CAP = new_cap;
    }

    /* Compute terms iteratively from _CACHE_LEN+1 up to n */
    for (idx_t i = _CACHE_LEN + 1; i <= n; ++i) {
        seq_t pos = 0; /* sum of positive contributions */
        seq_t neg = 0; /* sum of negative contributions (absolute) */

        for (int k = 0; k < 10; ++k) {
            /* term a(i‑1‑k) */
            idx_t idx = i - 1 - (idx_t)k;
            seq_t term = _CACHE[idx];
            int coeff = _COEFF[k];
            if (coeff > 0) {
                pos += term * (seq_t)(coeff);
            } else {
                neg += term * (seq_t)(-coeff);
            }
        }
        /* pos >= neg for all valid n; subtraction is safe for unsigned type */
        _CACHE[i] = pos - neg;
    }
    _CACHE_LEN = n;
    return _CACHE[n];
}
