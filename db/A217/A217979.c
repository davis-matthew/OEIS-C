/*
 * A217979  Number of n X 5 arrays of the minimum value of corresponding
 *          elements and their horizontal, vertical, diagonal or antidiagonal
 *          neighbors in a random 0..1 n X 5 array.
 * https://oeis.org/A217979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) +10*a(n-2) +a(n-3) -28*a(n-4) -58*a(n-5)
 *            +41*a(n-6) +39*a(n-7) -50*a(n-8) -9*a(n-9) +100*a(n-10)
 *            +368*a(n-11) -439*a(n-12) -51*a(n-13) +370*a(n-14) -249*a(n-15)
 *            -56*a(n-16) +106*a(n-17) -33*a(n-18) -7*a(n-19) +6*a(n-20)
 *            +a(n-21); a(1) = 10, a(2) = 10, a(3) = 100, a(4) = 362, a(5)...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const int _coeff[21] = {
    3, 10, 1, -28, -58, 41, 39, -50, -9, 100,
    368, -439, -51, 370, -249, -56, 106, -33, -7, 6, 1
};

/* initial terms a(1)..a(21) */
static const seq_t _init[21] = {
    10ULL, 10ULL, 100ULL, 362ULL, 1942ULL, 8872ULL, 43258ULL,
    205446ULL, 986288ULL, 4714274ULL, 22573862ULL, 108013892ULL,
    516992146ULL, 2474201538ULL, 11841537864ULL, 56672476422ULL,
    271231398802ULL, 1298094250056ULL, 6212595977182ULL,
    29733069165822ULL, 142300516403984ULL
};

static seq_t * _cache = NULL;
static size_t   _cache_len = 0;

static void _ensure_cache(size_t n)
{
    if (_cache_len >= n) return;
    size_t new_len = (size_t)n;
    seq_t *tmp = (seq_t*)realloc(_cache, new_len * sizeof(seq_t));
    if (!tmp) return; /* allocation failure: caller will return 0 */
    _cache = tmp;
    /* fill missing entries for the first 21 terms */
    for (size_t i = _cache_len; i < 21 && i < new_len; ++i)
        _cache[i] = _init[i];
    /* compute remaining terms up to new_len */
    for (size_t i = _cache_len; i < new_len; ++i) {
        if (i < 21) continue; /* already set */
        /* compute a(i+1) using recurrence */
        if (sizeof(seq_t) <= sizeof(uint64_t)) {
            /* 64-bit or smaller: use signed __int128 for accumulation */
            __int128 sum = 0;
            for (int k = 0; k < 21; ++k) {
                seq_t term = _cache[i - k - 1];
                int c = _coeff[k];
                sum += (__int128)term * c;
            }
            _cache[i] = (seq_t)sum;
        } else {
            /* larger type: use separate positive and negative sums */
            seq_t pos = 0, neg = 0;
            for (int k = 0; k < 21; ++k) {
                seq_t term = _cache[i - k - 1];
                int c = _coeff[k];
                seq_t prod = term * (seq_t)(c > 0 ? c : -c);
                if (c > 0)
                    pos += prod;
                else
                    neg += prod;
            }
            /* final result is guaranteed non‑negative */
            _cache[i] = pos - neg;
        }
    }
    _cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* undefined below offset; return 0 */

    _ensure_cache((size_t)n);
    if (_cache == NULL) return 0; /* allocation failure */

    return _cache[n - 1];
}
