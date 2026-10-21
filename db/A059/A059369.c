/*
 * A059369  Triangle of numbers T(n,k) = T(n-1,k-1) + ((n+k-1)/k)*T(n-1,k), n
 *          >= 1, 1 <= k <= n, with T(n,1) = n!, T(n,n) = 1; read from right
 *          to left.
 * https://oeis.org/A059369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) = T(r, k) where r*(r+1)/2 >= n, T(n,1)=n!, T(n,n)=1,
 *            T(n,k)=T(n-1,k-1)+((n+k-1)/k)*T(n-1,k), read triangle rows
 *            right‑to‑left
 * Range:     u8 n <= 19, u16 n <= 43, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For all compilations we use
   seq_t      – unsigned integer of 8–128 bits or an arbitrary‑precision class
   idx_t      – uint64_t                                           */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/factorial.h"

static seq_t **_rows = NULL;   /* 1‑based rows, _rows[0] unused */
static idx_t  _max_row = 0;    /* largest row computed so far */

/* Ensure rows 1..n are computed.  Allocation is done with casts to keep the
   code both C and C++ compatible. */
static void _ensure_rows(idx_t n)
{
    if (n <= _max_row)
        return;

    /* Reallocate the array of row pointers.  Use a cast for C++. */
    seq_t **tmp = (seq_t **)realloc(_rows, (size_t)(n + 1) * sizeof(seq_t *));
    if (!tmp)
        abort();               /* Out of memory */
    _rows = tmp;

    /* Compute new rows one by one.  Each row is stored as a 0‑unused,
       1‑based array of seq_t values. */
    for (idx_t i = _max_row + 1; i <= n; ++i)
    {
        /* Allocate i+1 entries, index 0 unused.  Use a cast for C++. */
        seq_t *row = (seq_t *)calloc((size_t)(i + 1), sizeof(seq_t));
        if (!row)
            abort();

        _rows[i] = row;

        /* Boundary values */
        row[1] = factorial(i);   /* T(i,1) */
        row[i] = (seq_t)1;       /* T(i,i) */

        /* Interior values */
        for (idx_t k = 2; k < i; ++k)
        {
            seq_t prev_left = _rows[i - 1][k - 1];
            seq_t prev_right = _rows[i - 1][k];

            /* Compute ((i+k-1)/k) * prev_right without overflow: */
            seq_t factor = (seq_t)(i + k - 1);
            seq_t prod = factor * prev_right;  /* type seq_t * seq_t -> seq_t */
            seq_t addend = prod / (seq_t)k;    /* exact division */

            row[k] = prev_left + addend;
        }
    }

    _max_row = n;
}

/* Main sequence accessor.  n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;    /* undefined for n<1, return 0 */

    /* Find the row r such that r*(r+1)/2 >= n.  r is small (≈ sqrt(2n)). */
    idx_t r = 1;
    while ((r * (r + 1ULL)) / 2ULL < n)
        ++r;

    /* Number of terms before this row. */
    idx_t prev = ((r - 1ULL) * r) / 2ULL;

    /* Position inside the row, 1‑based, counting from the rightmost element. */
    idx_t pos = n - prev;                 /* 1‑based */
    idx_t k   = r - pos + 1ULL;            /* column index from left (1‑based) */

    _ensure_rows(r);
    return _rows[r][k];
}
