/*
 * A188881  Triangle of coefficients arising from an expansion of Integral(
 *          exp(exp(exp(x))), dx).
 * https://oeis.org/A188881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 37, u32 n <= 81, u64 n <= 195,
 *            u128 n <= 508, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A188881 – Triangle of coefficients  
   Flattened by rows, 0‑indexed rows/columns:
       T(0,0)=1
       T(n,k) = k*T(n-1,k-1) + n*T(n-1,k),  for n≥1 and 0≤k≤n
   a(n) = T(r,k) where r is the smallest row with
          (r+1)(r+2)/2 ≥ n  and k = n - r(r+1)/2 - 1
*/

#include <stdlib.h>

static seq_t **triangle = NULL;        /* pointers to each row of the triangle      */
static size_t  max_row   = (size_t)-1;  /* index of the last generated row (wraps to SIZE_MAX) */
static size_t  capacity  = 0;           /* allocated capacity for triangle pointers */

/* build all rows up to and including target_row (0‑based) */
static void ensure_triangle(size_t target_row)
{
    if (triangle == NULL) {              /* first time: create row 0 */
        capacity = 16;
        triangle = (seq_t **)malloc(capacity * sizeof(seq_t *));
        if (triangle == NULL)
            exit(EXIT_FAILURE);
        triangle[0] = (seq_t *)malloc(sizeof(seq_t));
        triangle[0][0] = (seq_t)1;       /* T(0,0)=1 */
        max_row = 0;
    }

    while (max_row < target_row) {       /* generate next row */
        size_t row = max_row + 1;        /* row index of the new row */
        size_t len = row + 1;            /* number of elements in the new row (k=0..row) */
        seq_t *new_row = (seq_t *)malloc(len * sizeof(seq_t));
        seq_t *prev_row = triangle[max_row];
        size_t prev_len = max_row + 1;   /* length of previous row */

        for (size_t k = 0; k < len; ++k) {
            seq_t part1 = (k > 0) ? ((seq_t)k * prev_row[k - 1]) : (seq_t)0;
            seq_t part2 = (k < prev_len) ? ((seq_t)row * prev_row[k]) : (seq_t)0;
            new_row[k] = part1 + part2;
        }

        /* enlarge pointer array if necessary */
        if (max_row + 1 >= capacity) {
            capacity = capacity ? capacity * 2 : 16;
            triangle = (seq_t **)realloc(triangle, capacity * sizeof(seq_t *));
            if (triangle == NULL)
                exit(EXIT_FAILURE);
        }
        triangle[++max_row] = new_row;
    }
}

/* public function required by the build system */
seq_t a(idx_t n)
{
    /* a(1) is the first element (row 0, k=0) */
    if (n == 0)            /* offset is 1; behavior for n==0 is irrelevant */
        return (seq_t)0;

    size_t N = (size_t)n;   /* cast to size_t for arithmetic */

    /* locate the row r such that (r+1)(r+2)/2 >= N */
    size_t r = 0;
    while (((r + 1) * (r + 2)) / 2 < N)
        ++r;

    ensure_triangle(r);

    size_t elements_before = (r * (r + 1)) / 2;
    size_t k = N - elements_before - 1;   /* 0‑based column within the row */

    return triangle[r][k];
}
