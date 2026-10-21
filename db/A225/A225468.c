/*
 * A225468  Triangle read by rows, S_3(n, k) where S_m(n, k) are the
 *          Stirling-Frobenius subset numbers of order m; n >= 0, k >= 0.
 * https://oeis.org/A225468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 80, u64 n <= 213,
 *            u128 n <= 601, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A225468 – “Triangle read by rows, S₃(n,k)”
    The triangle T(n,k) satisfies
        T(0,0) = 1
        T(n,k) = (2 + 3·k)·T(n‑1,k) + T(n‑1,k‑1)          (0 ≤ k ≤ n)
    The flattened sequence a(n) enumerates the triangle row‑by‑row:
        a(n) = T(r, c)          where  r(r+1)/2 ≤ n < (r+1)(r+2)/2
        c = n – r(r+1)/2
    The function below keeps a static cache of computed rows; each call is
    O(r²) with r ≈ √(2n).  It works for any seq_t/idx_t supplied by the
    build and for arbitrary‑precision seq_t classes.  All memory
    allocations are explicitly cast for C++ compliance. */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, calloc, realloc */

seq_t a(idx_t n)
{
    /* ---- static cache of triangle rows --------------------------------- */
    static seq_t **table = NULL;   /* table[row][k] = T(row,k)           */
    static size_t max_row = 0;     /* highest row already computed        */

    /* ----- initialise cache on first call ------------------------------ */
    if (table == NULL) {
        table = (seq_t**)calloc(1, sizeof(seq_t*));   /* row 0 pointer      */
        table[0] = (seq_t*)calloc(1, sizeof(seq_t));  /* row 0 contents     */
        table[0][0] = (seq_t)1;                       /* T(0,0) = 1         */
        max_row = 0;
    }

    /* ----- find row r that contains the flat index n ------------------ */
    idx_t r = 0;
    /* use 128–bit arithmetic to avoid overflow for very large n          */
    while ( ( (__uint128_t)(r + 1) * (r + 2) / 2 ) <= n )
        ++r;

    /* offset of the first element of row r and column c                 */
    idx_t offset = (idx_t)(((__uint128_t)r * (r + 1) / 2));
    idx_t c = n - offset;

    /* ----- compute missing rows up to r -------------------------------- */
    if ( (size_t)r > max_row ) {
        size_t new_cap = (size_t)r + 1;
        table = (seq_t**)realloc(table, new_cap * sizeof(seq_t*));
        for (size_t i = max_row + 1; i <= (size_t)r; ++i) {
            table[i] = (seq_t*)calloc(i + 1, sizeof(seq_t));
            for (idx_t k = 0; k <= (idx_t)i; ++k) {
                seq_t val_front = 0;   /* T(i‑1,k) if k≤i‑1            */
                seq_t val_back  = 0;   /* T(i‑1,k‑1) if k≥1            */
                if (k <= (idx_t)(i - 1)) val_front = table[i - 1][k];
                if (k >= 1)             val_back  = table[i - 1][k - 1];
                /* factor = 2 + 3*k  (small integer)                     */
                seq_t factor = (seq_t)(2 + 3 * k);
                seq_t mult   = factor * val_front;   /* seq_t arithmetic  */
                table[i][k] = mult + val_back;
            }
        }
        max_row = r;
    }

    return table[r][c];
}
