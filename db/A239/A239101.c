/*
 * A239101  Riordan array read by rows, corresponding to array in A180562.
 * https://oeis.org/A239101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, more
 * Formula:   a(n) = the n-th term obtained by reading the triangle T(n,k)
 *            (left‑to‑right) where T(0,0) = 1 T(n,0) = 2*T(n,1) for n>0
 *            T(n,k) = T(n-1,k-1) - T(n-1,k) + T(n-1,k+1) + T(n,k+1) for k>0
 *            with T(n,k)=0 if k<0 or k>n. The sequence begins:
 *            1,2,1,4,2,1,10,5,2,1,26,13,6,2,1,70,35,16,7,2,1,192,96,45,
 * Range:     u8 n <= 27, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* 64‑bit helper to compute m*(m+1)/2 without overflow for m <= 2^32 */
static inline idx_t triangular(idx_t m)
{
    /* use division first */
    if ((m & 1ULL) == 0)
        return (m / 2ULL) * (m + 1ULL);
    else
        return m * ((m + 1ULL) / 2ULL);
}

/* storage for the computed rows of the triangle */
static seq_t **_rows   = NULL;  /* pointer array */
static size_t _rows_cap = 0;    /* capacity of the pointer array */
static size_t _rows_used = 0;   /* number of rows actually computed */

/* ensure that rows 0..m are computed */
static void _ensure_rows(idx_t m)
{
    if (m < _rows_used)
        return;

    /* grow pointer array if necessary */
    if (m + 1 > _rows_cap) {
        size_t new_cap = _rows_cap ? _rows_cap * 2 : 1;
        while (new_cap < (size_t)m + 1)
            new_cap <<= 1;
        seq_t **tmp = (seq_t **)realloc(_rows, new_cap * sizeof(seq_t *));
        if (!tmp)
            exit(1);                /* out of memory */
        _rows = tmp;
        _rows_cap = new_cap;
    }

    for (idx_t i = _rows_used; i <= m; ++i) {
        seq_t *row = (seq_t *)malloc((i + 1) * sizeof(seq_t));
        if (!row)
            exit(1);                /* out of memory */
        _rows[i] = row;

        if (i == 0) {
            row[0] = (seq_t)1;      /* T(0,0) */
            continue;
        }

        /* T(i,i) = T(i-1,i-1) */
        row[i] = _rows[i-1][i-1];

        /* compute k = i-1 .. 1 using the recurrence */
        for (idx_t k = i-1; k >= 1; --k) {
            seq_t val = _rows[i-1][k-1] - _rows[i-1][k];
            if (k + 1 <= i-1)
                val += _rows[i-1][k+1];
            val += row[k+1];
            row[k] = val;
        }

        /* T(i,0) = 2 * T(i,1) */
        row[0] = row[1] + row[1];   /* 2*row[1] */
    }
    _rows_used = m + 1;
}

/* find the row index m such that m*(m+1)/2 <= n < (m+1)*(m+2)/2 */
static idx_t _find_row(idx_t n)
{
    /* start with a floating‑point estimate */
    double d = sqrt((double)n * 8.0 + 1.0);
    idx_t m = (idx_t)((d - 1.0) / 2.0);

    /* adjust down/up to precisely locate m */
    while (triangular(m+1) <= n)
        ++m;
    while (triangular(m) > n)
        --m;
    return m;
}

/* public function required by the build system */
seq_t a(idx_t n)
{
    idx_t m   = _find_row(n);                 /* row number */
    idx_t pos = n - triangular(m);           /* position inside the row */
    _ensure_rows(m);                          /* compute rows as needed */
    return _rows[m][pos];
}
