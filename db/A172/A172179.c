/*
 * A172179  (1,[99n+1]) Pascal Triangle.
 * https://oeis.org/A172179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element read row‑wise from the triangle T(n,k)
 *            defined by T(1,0) = 1 T(2,0) = 1, T(2,1) = 100 T(n,k) = T(n-1,k)
 *            + 2·T(n-1,k-1) – T(n-2,k-1) – T(n-2,k-2) for n ≥ 3, 0 < k < n
 *            with T(n,k)=0 if k<0 or k≥n. The index n is 1‑based: a(1)=1,
 *            a(2)=1, a(3)=100, …
 * Range:     u8 n <= 8, u16 n <= 84, u32 n <= 448, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t */

/* A safe upper bound for the number of rows needed for n≤10000.
   Each row fills a triangle: r*(r+1)/2 entries.
   For n≈100 000 000 we would need r≈14142; we choose 2048 for safety. */
#define MAX_ROWS 2048

/* Triangle storage (indexed by row i, column j; 1‑based row). */
static seq_t triangle[MAX_ROWS][MAX_ROWS];
/* Highest row computed so far. */
static int computed_row = 0;

/* Compute rows up to 'up_to' (inclusive) using the recurrence. */
static void compute_up_to(int up_to)
{
    for (int i = computed_row + 1; i <= up_to; ++i) {
        /* Each row i has i elements (columns 0 .. i‑1). */
        for (int j = 0; j < i; ++j) {
            if (j == 0) {                      /* first element of every row */
                triangle[i][j] = 1;
            } else if (i == 2 && j == 1) {     /* special base case */
                triangle[i][j] = 100;
            } else {
                /* Use 64‑bit arithmetic for intermediate values
                   to avoid overflow and underflow for unsigned seq_t. */
                uint64_t val = 0;
                val += (uint64_t)triangle[i-1][j];                         /* T(n-1,k)    */
                val += 2ULL * (uint64_t)triangle[i-1][j-1];                /* 2·T(n-1,k-1) */
                val -= (uint64_t)triangle[i-2][j-1];                       /* –T(n-2,k-1) */
                if (j >= 2)
                    val -= (uint64_t)triangle[i-2][j-2];                    /* –T(n-2,k-2) */
                triangle[i][j] = (seq_t)val;
            }
        }
    }
    if (up_to > computed_row) computed_row = up_to;
}

/* Compute the n‑th term of sequence A172179. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* offset starts at 1 */

    /* Find the row r such that r*(r+1)/2 ≥ n. */
    int r = 1;
    while ((uint64_t)r * (r + 1u) / 2u < n) ++r;

    /* Index of the element within row r (1‑based). */
    uint64_t prev = (uint64_t)(r - 1) * r / 2u;
    uint64_t col  = (uint64_t)n - prev;          /* 1 … r */
    int k = (int)col - 1;                        /* 0‑based column */

    if (r >= MAX_ROWS) { /* Should not happen for n≤10000 */
        /* Fallback: compute on the fly without memoization. */
        compute_up_to(r);
    } else {
        compute_up_to(r);
    }

    return triangle[r][k];
}
