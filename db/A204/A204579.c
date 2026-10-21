/*
 * A204579  Triangle read by rows: matrix inverse of the central factorial
 *          numbers T(2*n, 2*k) (A036969).
 * https://oeis.org/A204579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the row‑by‑row listing of the central factorial numbers
 *            T(n,k) (OEIS A036969). They satisfy the recurrence T(n,k) =
 *            T(n-1,k-1) - (n-1)^2 * T(n-1,k), with T(1,1) = 1. The triangle
 *            is read by rows, starting with a(1)=1. This function computes
 *            the term for any index n>=1 (a(0) is returned as 0)
 * Range:     i8 n <= 10, i16 n <= 21, i32 n <= 37, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static cache for triangular rows (index 1 .. _maxRow). */
static seq_t **_table = NULL;   /* _table[r] -> array of length r+1, indices 0 .. r */
static size_t _maxRow = 0;      /* largest row currently allocated */

/* Allocate and compute rows up to “need”.  The recurrence requires
   the previous row, so we build rows sequentially. */
static void ensure_rows(size_t need)
{
    if (need <= _maxRow) return;                      /* already ready */

    /* extend the array of row pointers to index “need”. */
    size_t newCap = need + 1;                         /* need+1 to allow index “need” */
    seq_t **tmp = (seq_t**)realloc(_table, newCap * sizeof(seq_t*));
    if (!tmp) abort();                                /* allocation failure */
    _table = tmp;

    /* Build all new rows. */
    for (size_t r = _maxRow + 1; r <= need; ++r) {
        /* allocate space for indices 0..r (index 0 unused). */
        seq_t *row = (seq_t*)malloc((r + 1) * sizeof(seq_t));
        if (!row) abort();

        if (r == 1) {
            /* base case */
            row[0] = 0;           /* unused */
            row[1] = 1;
        } else {
            seq_t *prev = _table[r - 1];
            seq_t sq = (seq_t)(r - 1) * (seq_t)(r - 1);
            row[0] = 0;           /* unused */
            for (size_t k = 1; k <= r; ++k) {
                seq_t term1 = 0;   /* T(n-1,k-1) */
                seq_t term2 = 0;   /* (n-1)^2 * T(n-1,k) */
                if (k > 1) term1 = prev[k - 1];
                if (k <= r - 1) term2 = sq * prev[k];
                row[k] = term1 - term2;
            }
        }
        _table[r] = row;
    }
    _maxRow = need;
}

/* Return a(n) for any n >= 1.  The triangular index n is mapped to
   row r and column c with r(r-1)/2 < n <= r(r+1)/2. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* offset is 1 */

    size_t N = (size_t)n;
    size_t r = 0;
    size_t sum = 0;
    while (sum < N) {
        ++r;
        sum += r;
    }
    size_t prevSum = sum - r;                /* indices covered before row r */
    size_t c = N - prevSum;                  /* 1 <= c <= r */

    ensure_rows(r);
    return _table[r][c];
}
