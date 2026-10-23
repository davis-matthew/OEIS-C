/*
 * A039913  Triangular "Fibonacci array".
 * https://oeis.org/A039913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(0, n)=Fib(n), a(1, n)=Fib(n+2), a(r, n)=a(r-1, n)+a(r-2, n) (r
 *            >= 2). The one‑dimensional sequence a(n) lists the entries a(r,
 *            n) of this triangular Fibonacci array along anti‑diagonals (for
 *            each k = r + n)
 * Range:     u8 n <= 91, u16 n <= 300, u32 n <= 1128, u64 n <= 4371,
 *            u128 n <= 7380, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fib_cache  = NULL;   /* Fibonacci numbers Fib(0)...Fib(last) */
static size_t fib_len     = 0;     /* size of fib_cache   */
static seq_t *table_cache = NULL;  /* flattened triangle table */
static size_t table_cap   = 0;     /* side length of table */

/* Ensure the triangle and Fibonacci tables are large enough for diagonal k. */
static void
ensure_cap(idx_t k)
{
    size_t new_cap = (size_t)k + 1;          /* we need rows 0..k */
    if (new_cap <= table_cap)
        return;

    /* Build Fibonacci numbers up to index new_cap + 2 (needed for base row 1). */
    size_t need_fib = new_cap + 3;           /* Fib(0) … Fib(new_cap+2) */
    if (!fib_cache || need_fib > fib_len) {
        fib_cache = (seq_t *)realloc(fib_cache, need_fib * sizeof(seq_t));
        if (fib_len < 2) {                 /* initialise first two Fibonacci values */
            fib_cache[0] = (seq_t)0;
            fib_cache[1] = (seq_t)1;
            fib_len = 2;
        }
        for (size_t i = fib_len; i < need_fib; ++i)
            fib_cache[i] = fib_cache[i-1] + fib_cache[i-2];
        fib_len = need_fib;
    }

    /* Allocate and fill the new triangular array. */
    seq_t *new_tbl = (seq_t *)malloc(new_cap * new_cap * sizeof(seq_t));
    for (size_t r = 0; r < new_cap; ++r) {
        for (size_t n = 0; n < new_cap; ++n) {
            if (r == 0)
                new_tbl[r * new_cap + n] = fib_cache[n];
            else if (r == 1)
                new_tbl[r * new_cap + n] = fib_cache[n + 2];
            else
                new_tbl[r * new_cap + n] =
                    new_tbl[(r-1) * new_cap + n] + new_tbl[(r-2) * new_cap + n];
        }
    }
    free(table_cache);
    table_cache = new_tbl;
    table_cap   = new_cap;
}

seq_t a(idx_t n)
{
    /* Find the diagonal index k such that (k+1)(k+2)/2 > n. */
    idx_t k = 0;
    while (((__int128)(k + 1) * (k + 2)) / 2 <= n)
        ++k;

    /* Number of entries before this diagonal. */
    idx_t prev_total = ((__int128)k * (k + 1)) / 2;
    idx_t pos        = n - prev_total;      /* position inside the diagonal (0‑based) */
    idx_t r = pos;                         /* row index */
    idx_t c = k - pos;                     /* column index (the n‑coordinate) */

    ensure_cap(k);

    size_t offset = (size_t)r * table_cap + (size_t)c;
    return table_cache[offset];
}
