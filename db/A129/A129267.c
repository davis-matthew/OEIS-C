/*
 * A129267  Triangle with T(n,k) = T(n-1,k-1) + T(n-1,k) - T(n-2,k-1) -
 *          T(n-2,k) and T(0,0)=1 .
 * https://oeis.org/A129267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 95, i16 n <= 333, i32 n <= 1195, i64 n <= 4502,
 *            i128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle A129267:  T(n,k)=T(n-1,k-1)+T(n-1,k)-T(n-2,k-1)-T(n-2,k), T(0,0)=1.
   a(n) is the triangle read by rows (row 0, then row 1, …).  The coefficient
   a(n) = T(r,c) where r = ⌊(√(1+8n)‑1)/2⌋ and c = n - r(r+1)/2. */

#include <stdlib.h>
#include <stdint.h>

/* integer square root of an unsigned __int128 value */
static inline idx_t isqrt_u128(__uint128_t x)
{
    idx_t lo = 0, hi = ~((idx_t)0);   /* hi = 2^64-1 */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t m2 = (__uint128_t)mid * mid;
        if (m2 <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* find row index r for linear index n */
static inline idx_t row_of(idx_t n)
{
    __uint128_t disc = (__uint128_t)8 * n + 1;
    idx_t sq = isqrt_u128(disc);            /* sqrt(disc) */
    return (sq - 1) / 2;                    /* floor((sqrt-1)/2) */
}

/* ---------- triangle storage ---------- */
static idx_t   rows_count   = 0;   /* number of rows currently computed */
static idx_t   rows_capacity = 0;
static seq_t **rows = NULL;         /* rows[i] points to array of length i+1 */

static void ensure_rows(idx_t need)
{
    if (need < rows_count) return;

    if (rows_capacity == 0) rows_capacity = 1;
    while (rows_capacity <= (size_t)need) rows_capacity <<= 1;
    if (rows_capacity != rows_count) {
        seq_t **tmp = (seq_t**)realloc(rows, rows_capacity * sizeof(seq_t*));
        rows = tmp;
    }

    for (idx_t i = rows_count; i <= need; ++i) {
        size_t len = i + 1;
        seq_t *row = (seq_t*)malloc(len * sizeof(seq_t));
        if (!row) abort();

        if (i == 0) {
            row[0] = 1;
        } else if (i == 1) {
            row[0] = 1;
            row[1] = 1;
        } else {
            seq_t *prev  = rows[i-1];
            seq_t *prev2 = rows[i-2];
            for (idx_t k = 0; k <= i; ++k) {
                seq_t val = 0;
                if (k > 0)                 val += prev[k-1];     /* T(i-1,k-1) */
                if (k <= i-1)              val += prev[k];      /* T(i-1,k)   */
                if (k > 0 && k <= i-1)     val -= prev2[k-1];   /* T(i-2,k-1) */
                if (k <= i-2)              val -= prev2[k];      /* T(i-2,k)   */
                row[k] = val;
            }
        }
        rows[i] = row;
    }
    rows_count = need + 1;
}

/* ---------- public API ---------- */
seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    idx_t start = (idx_t)(((__uint128_t)r * (r + 1)) / 2);
    idx_t c = n - start;

    ensure_rows(r);
    return rows[r][c];
}
