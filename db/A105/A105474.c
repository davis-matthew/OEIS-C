/*
 * A105474  Triangle read by rows: T(n,k) is number of compositions of n into
 *          k parts when each odd part can be of two kinds.
 * https://oeis.org/A105474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened triangle read by rows of T(n,k) = number
 *            of compositions of n into k parts when each odd part can appear
 *            in two different kinds. The flattened triangle is indexed as in
 *            OEIS: a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0),
 *            a(4)=T(2,1), a(5)=T(2,2), ... and so on. This us...
 * Range:     u8 n <= 39, u16 n <= 99, u32 n <= 339, u64 n <= 1157,
 *            u128 n <= 4334, big any n
 * Validated: standard, pass, 82 terms through n = 9102 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static inline idx_t
row_of(idx_t n)                 /* return r with r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
{
    idx_t low  = 0;
    idx_t high = 1;
    /* exponential search to find an upper bound */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        low  = high;
        high <<= 1;              /* double high */
    }
    /* binary search between low (inclusive) and high (inclusive) */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1) + 1; /* upper mid to avoid infinite loop */
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

/* Static storage for the triangle, grown as needed. */
static seq_t **table = NULL;   /* array of pointers to each row of the triangle */
static idx_t table_size = 0;   /* number of rows currently computed (first 0‑based) */

/* Ensure that rows 0..max_row are fully computed. */
static void
ensure_table(idx_t max_row)
{
    if (table_size > max_row)
        return;

    /* Grow the array of row pointers if necessary. */
    if (table_size == 0) {
        table = (seq_t **)calloc(1, sizeof(seq_t *));
    } else {
        seq_t **tmp = (seq_t **)realloc(table,
                                        (size_t)(max_row + 1) * sizeof(seq_t *));
        if (!tmp) abort();
        table = tmp;
    }

    /* Compute each new row. */
    for (idx_t r = table_size; r <= max_row; ++r) {
        /* Allocate space for T(r,0..r). */
        table[r] = seq_malloc(r + 1);
        if (!table[r]) abort();

        /* Base case. */
        if (r == 0) {
            table[0][0] = 1;
            continue;
        }

        table[r][0] = 0;   /* no compositions of positive value into 0 parts */

        /* Compute T(r,k) for k = 1..r. */
        for (idx_t k = 1; k <= r; ++k) {
            seq_t sum = 0;
            /* Current part size ranges from 1 to r-k+1 to keep remaining sum ≥ k-1. */
            for (idx_t part = 1; part <= r - k + 1; ++part) {
                seq_t val = table[r - part][k - 1];
                if (part & 1u)          /* odd part has two kinds */
                    val += val;         /* multiply by 2 */
                sum += val;
            }
            table[r][k] = sum;
        }
    }

    table_size = max_row + 1;
}

/* Required function. */
seq_t
a(idx_t n)
{
    idx_t row = row_of(n);                     /* 0‑based row index */
    idx_t col = n - row * (row + 1) / 2;       /* 0‑based column index in that row */

    ensure_table(row);                         /* compute all rows up to this one if necessary */

    return table[row][col];
}
