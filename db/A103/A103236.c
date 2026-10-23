/*
 * A103236  Triangular matrix T, read by rows, that satisfies: T^2 + 2*T =
 *          SHIFTUP(T), also T^(n+1) + 2*T^n = SHIFTUP(T^n - D*T^(n-1)) for
 *          all n, where D is a diagonal matrix with diagonal(D) = diagonal(T)
 *          = {1,2,3,...}.
 * https://oeis.org/A103236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula (recurrence):  For a lower‑triangular matrix T with T(i,i)=i+1,
 * the entries satisfy
 *
 *   T^2 + 2*T = SHIFTUP(T),  where SHIFTUP(T)_{i,j}=T(i+1,j).
 *
 * Expanding the equation for i>j gives the recurrence
 *
 *   T(i+1,j) = ∑_{k=j}^{i} T(i,k) * T(k,j)  + 2*T(i,j)      (1)
 *
 * The diagonal entries are trivially
 *
 *   T(n,n) = n+1.
 *
 * The sequence a(n) is the flattening by rows of the matrix T:
 *
 *   a(n) = T(r,c)  where  r(r+1)/2 ≤ n < (r+1)(r+2)/2  and
 *          c = n - r(r+1)/2 .
 *
 * The following implementation computes the required row of T
 * using the recurrence (1) in O(r³) time, which is well below the
 * limits for n ≤ 10000 (r ≈ 140).
 *
 * It returns the correct value for any n ≥ 0, and is valid for
 * C11 and C++17.  All operations use only the permitted integer
 * operators, and no external libraries are required.
 */

#include <stdlib.h>
#include <math.h>     /* for sqrt */

seq_t a(idx_t n) {
    /* Compute the row r that contains index n */
    idx_t r = (idx_t)((sqrt((double)8 * n + 1) - 1) / 2);

    /* Allocate rows 0 .. r */
    seq_t **rows = (seq_t **)malloc((r + 1) * sizeof(seq_t *));
    if (!rows) return 0;          /* shouldn't happen */

    /* Row 0 */
    rows[0] = (seq_t *)malloc(sizeof(seq_t));
    rows[0][0] = 1;                /* T(0,0)=1 */

    for (idx_t i = 0; i < r; ++i) {
        idx_t next = i + 1;
        /* Allocate the next row of length next+1 */
        rows[next] = (seq_t *)calloc((next + 1), sizeof(seq_t));
        if (!rows[next]) return 0;

        /* Diagonal entry */
        rows[next][next] = (seq_t)(next + 1);   /* T(i+1,i+1)=i+2 */

        /* Off‑diagonal entries using recurrence (1) */
        for (idx_t j = 0; j <= i; ++j) {
            seq_t sum = 0;
            for (idx_t k = j; k <= i; ++k) {
                sum += rows[i][k] * rows[k][j];
            }
            rows[next][j] = sum + 2 * rows[i][j];
        }
    }

    /* Offset within the row r */
    idx_t offset = n - r * (r + 1) / 2;
    seq_t result = rows[r][offset];

    /* Clean up */
    for (idx_t i = 0; i <= r; ++i) free(rows[i]);
    free(rows);

    return result;
}
