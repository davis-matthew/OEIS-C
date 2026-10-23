/*
 * A019538  Triangle of numbers T(n,k) = k!*Stirling2(n,k) read by rows (n >=
 *          1, 1 <= k <= n).
 * https://oeis.org/A019538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) = entry T(n,k) = k!*Stirling2(n,k) read by rows. * Triangle
 *            computed by recurrence: * T(0,0) = 1 * T(n,k) = k*(T(n-1,k-1) +
 *            T(n-1,k)) for n ≥ 1, 1 ≤ k ≤ n
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 73, u64 n <= 184,
 *            u128 n <= 514, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Memoised triangle: rows 0..max_row, each row 0..row  (index 0 unused for k=0 except row 0) */
static seq_t **T_table = NULL;
static idx_t  max_row = 0;

/* Initialise base row 0 */
static void init_base(void)
{
    if (T_table == NULL) {
        /* Allocate array of pointers for rows, start with row 0 */
        T_table = (seq_t**)malloc(sizeof(seq_t*));
        if (!T_table) exit(EXIT_FAILURE);

        /* Row 0 contains a single element T(0,0) = 1 */
        T_table[0] = (seq_t*)malloc(sizeof(seq_t));
        if (!T_table[0]) exit(EXIT_FAILURE);
        T_table[0][0] = (seq_t)1;

        max_row = 0;
    }
}

/* Grow triangle up to the requested row (inclusive) */
static void ensure_rows_up_to(idx_t r)
{
    if (r <= max_row) return;          /* already computed */

    init_base();                         /* guarantee base exists */

    /* Resize the row‑pointer array */
    T_table = (seq_t**)realloc(T_table, sizeof(seq_t*) * (size_t)(r + 1));
    if (!T_table) exit(EXIT_FAILURE);

    /* Compute new rows */
    for (idx_t row = max_row + 1; row <= r; ++row) {
        /* Allocate storage for this row; indices 0..row */
        T_table[row] = (seq_t*)malloc(sizeof(seq_t) * (size_t)(row + 1));
        if (!T_table[row]) exit(EXIT_FAILURE);

        /* The k = 0 entry is 0 (unused) */
        T_table[row][0] = (seq_t)0;

        /* Compute values for k = 1 .. row */
        for (idx_t k = 1; k <= row; ++k) {
            seq_t left  = (k <= row-1) ? T_table[row-1][k]   : (seq_t)0; /* T(n-1,k) */
            seq_t right = (k-1 <= row-1) ? T_table[row-1][k-1] : (seq_t)0; /* T(n-1,k-1) */
            T_table[row][k] = (seq_t)k * (left + right);
        }
    }
    max_row = r;
}

/* Smallest r such that r*(r+1)/2 ≥ n  (row index for element n) */
static idx_t find_row(idx_t n)
{
    idx_t r = 0;
    while ((r * (r + 1)) / 2 < n) ++r;
    return r;
}

/* Return the n‑th term of A019538 (1‑based).  n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for 0 */

    /* Locate row r and column k of the n‑th element (row‑major order) */
    idx_t r = find_row(n);
    idx_t k = n - (r - 1) * r / 2; /* k ranges 1..r */

    /* Compute triangle up to row r if not already present */
    ensure_rows_up_to(r);

    return T_table[r][k];
}
