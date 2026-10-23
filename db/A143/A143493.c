/*
 * A143493  Unsigned 4-Stirling numbers of the first kind.
 * https://oeis.org/A143493
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 31, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A143493: Unsigned 4‑Stirling numbers of the first kind
 *  Sequence a(n) is the triangle read row‑by‑row, starting with
 *  row n = 4.  Let T(n,k) be the unsigned 4‑Stirling number of
 *  the first kind, defined by:
 *
 *        T(n,k) = T(n‑1,k‑1) + (n‑1)·T(n‑1,k)   for n > 4,
 *        T(4,4) = 1,
 *        T(n,3) = 0  (for all n),
 *        T(n,k) = 0  for k > n.
 *
 *  The value a(n) is a((n‑4)+1) in the b‑file, i.e. the nth
 *  entry of the triangle read row by row.
 *
 *  Implementation uses a lazy memoised table of T(n,k) up to the
 *  required row.  The table is built only when needed and
 *  re‑used for subsequent calls.  No floating point or large
 *  libraries are required.
 */

#include <stdlib.h>

static seq_t **T_table = NULL;     /* rows of the triangle, index by n */
static idx_t  T_max_row = 3;       /* largest row currently computed */
static idx_t  T_cap     = 0;       /* current allocated capacity for rows */

/* Allocate or grow the triangle table up to row `need`.
 * All rows 4..need will be filled.               */
static void ensure_row(idx_t need)
{
    if (need <= T_max_row)
        return;

    /* Grow the row array to at least 'need' (plus one for 0-based safety) */
    idx_t new_cap = (T_cap ? T_cap : 4);
    while (new_cap <= need)
        new_cap <<= 1;   /* double until big enough */

    T_table = (seq_t **)realloc(T_table, sizeof(*T_table) * (new_cap + 1));
    for (idx_t i = T_max_row + 1; i <= new_cap; ++i)
        T_table[i] = NULL;      /* initialise new row pointers */

    /* Build missing rows */
    for (idx_t n = T_max_row + 1; n <= need; ++n) {
        size_t len = n - 3;     /* number of entries in row n (k = 4..n) */
        T_table[n] = (seq_t *)malloc(sizeof(seq_t) * len);
        if (!T_table[n])
            return;            /* out of memory, caller will get 0 */

        if (n == 4) {          /* base case T(4,4) = 1 */
            T_table[4][0] = (seq_t)1;
            continue;
        }

        /* Compute all T(n,k) for 4 <= k <= n using recurrence */
        for (idx_t k = 4; k <= n; ++k) {
            seq_t val1 = 0;                 /* T(n-1,k-1) */
            seq_t val2 = 0;                 /* T(n-1,k)   */

            if (k - 1 >= 4 && k - 1 <= n - 1)
                val1 = T_table[n - 1][(k - 1) - 4];   /* k-1 -> index (k-5) */

            if (k <= n - 1)
                val2 = T_table[n - 1][k - 4];         /* k   -> index (k-4) */

            T_table[n][k - 4] = val1 + (n - 1) * val2;
        }
    }

    T_max_row = need;
    T_cap     = new_cap;
}

/* Return the nth term of A143493 (offset n = 4). */
seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;          /* undefined below the offset */

    /* Position of n in the flattened triangle (1‑based) */
    idx_t pos = n - 3;

    /* Find the row r such that (r‑3)(r‑2)/2 >= pos */
    idx_t r = 4;
    while ((unsigned __int128)(r - 3) * (r - 2) / 2 < pos)
        ++r;

    /* Number of entries before row r */
    idx_t before = (r - 4) * (r - 3) / 2;

    /* Column offset inside row r (0‑based) */
    idx_t col_off = pos - before - 1;

    ensure_row(r);

    return T_table[r][col_off];
}
