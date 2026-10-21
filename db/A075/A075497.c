/*
 * A075497  Stirling2 triangle with scaled diagonals (powers of 2).
 * https://oeis.org/A075497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the flattened list of a(n,m)=2^(n-m)*S(n,m), where
 *            S(n,m) are Stirling numbers of the second kind. Recurrence for
 *            the triangle coefficients: a(1,1) = 1 a(n,1) = 2*a(n-1,1) (n ≥
 *            2) a(n,m) = 2*m*a(n-1,m) + a(n-1,m-1) (n ≥ 2, 2 ≤ m ≤ n) The
 *            sequence a(n) is the triangle read row‑by‑row from...
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 94, u64 n <= 237,
 *            u128 n <= 676, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define MAX_ROW 150                     /* enough for n ≤ 10000 – rows ≈141 */
#define MAX_TERMS ((MAX_ROW + 1) * (MAX_ROW + 2) / 2)   /* ≈11325 */

/* static storage for incremental construction of the triangle */
static seq_t prev_row[MAX_ROW + 2];     /* previous row, 1‑based index */
static seq_t new_row[MAX_ROW + 2];      /* temporary for the next row   */
static seq_t cache[MAX_TERMS];          /* flattened sequence, 1‑based */ 
static idx_t cache_size = 0;            /* number of terms currently cached */
static idx_t rows_computed = 0;         /* number of rows that have been produced */

/* generate rows until the cache contains at least n terms */
static void ensure_terms(idx_t n)
{
    while (cache_size < n) {
        ++rows_computed;                     /* row index i */
        idx_t i = rows_computed;

        if (i == 1) {
            new_row[1] = (seq_t)1;           /* a(1,1) = 1 */
        } else {
            /* a(i,1) */
            new_row[1] = (seq_t)2 * prev_row[1];
            /* middle elements a(i,j) */
            for (idx_t j = 2; j < i; ++j)
                new_row[j] = (seq_t)2 * j * prev_row[j] + prev_row[j - 1];
            /* a(i,i) = a(i-1,i-1) */
            new_row[i] = prev_row[i - 1];
        }

        /* store the new row into the flattened cache and copy to prev_row */
        for (idx_t j = 1; j <= i; ++j) {
            cache[cache_size++] = new_row[j];
            prev_row[j] = new_row[j];
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* values below the offset are irrelevant */
    ensure_terms(n);
    return cache[n - 1];                  /* cache[0] holds a(1) */
}
