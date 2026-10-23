/*
 * A178304  Triangle T(n,m) = 1 + f(n+1)*(f(m+1) + f(n-m+1) - 1 - f(n+1)),
 *          read by rows, where f(.)=A002487(.).
 * https://oeis.org/A178304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = T(row,col) where * T(n,m) = 1 + f(n+1)*(f(m+1) + f(n-m+1)
 *            - 1 - f(n+1)) * f is Stern's diatomic sequence (A002487) * row
 *            is the largest r with r*(r+1)/2 <= n * col = n - r*(r+1)/2 *
 *            (offset 0)
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ---- Stern's diatomic sequence ------------------------------------- */
static seq_t *f_cache = NULL;
static idx_t  f_cache_size = 0;

static seq_t stern_f(idx_t k)
{
    /* Lazy initialization */
    if (f_cache == NULL) {
        f_cache_size = 3;
        f_cache = (seq_t *)calloc(f_cache_size, sizeof(seq_t));
        if (!f_cache) { /* malloc failure: abort computation */
            abort();
        }
        f_cache[0] = 0;      /* index 0 unused but defined */
        f_cache[1] = 1;
        f_cache[2] = 1;
    }

    /* Grow cache if necessary */
    if (k >= f_cache_size) {
        idx_t new_size = k + 1;
        seq_t *tmp = (seq_t *)realloc(f_cache, new_size * sizeof(seq_t));
        if (!tmp) {
            abort();
        }
        /* zero‑initialize new slots */
        for (idx_t i = f_cache_size; i < new_size; ++i) {
            tmp[i] = 0;
        }
        f_cache = tmp;
        f_cache_size = new_size;
    }

    /* Return known value */
    if (f_cache[k] != 0 || k == 0 || k == 1 || k == 2) {
        return f_cache[k];
    }

    /* Recursive computation with memoisation */
    seq_t val;
    if ((k & 1ULL) == 0ULL) {            /* even */
        val = stern_f(k >> 1);
    } else {                            /* odd */
        val = stern_f(k >> 1) + stern_f((k >> 1) + 1ULL);
    }

    f_cache[k] = val;
    return val;
}

/* ---- Find row index for flattened triangle ------------------------ */
static uint64_t row_from_index(uint64_t n)
{
    uint64_t lo = 0;
    uint64_t hi = n + 1;      /* Upper bound guarantees hi large enough */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1ULL) / 2ULL;
        if (tri <= n) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

/* ---- Triangle entry ------------------------------------------------ */
static seq_t T(uint64_t r, uint64_t m)
{
    seq_t ar   = stern_f(r + 1ULL);      /* f(r+1)   */
    seq_t bm   = stern_f(m + 1ULL);      /* f(m+1)   */
    seq_t bnm  = stern_f(r - m + 1ULL);  /* f(r-m+1) */

    /* 1 + f(r+1) * ( f(m+1) + f(r-m+1) - 1 - f(r+1) ) */
    seq_t sum = bm + bnm - 1 - ar;
    return 1 + ar * sum;
}

/* ---- Public sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    uint64_t row = row_from_index((uint64_t)n);
    uint64_t col = n - (row * (row + 1ULL) / 2ULL);
    return T(row, col);
}
