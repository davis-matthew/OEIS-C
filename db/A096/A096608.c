/*
 * A096608  Triangle read by rows: T(n,k)=number of Catalan knight paths in
 *          right half-plane from (0,0) to (n,k), for 0 <= k <= 2n, n >= 0.
 *          (See A096587 for the definition of a Catalan knight.)
 * https://oeis.org/A096608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(floor(sqrt(n)), n - floor(sqrt(n))^2) * where T(n,k) is
 *            defined by the recurrence: * T(0,0)=1, all other T(0,k)=0 *
 *            T(1,0)=0, T(1,1)=0, T(1,2)=1, all other T(1,k)=0 * For n >= 2 *
 *            T(n,0) = 2*T(n-2,1) + 2*T(n-1,2) * T(n,1) = T(n-2,0) + T(n-2,2)
 *            + T(n-1,3) + T(n-1,1) * For 2 <= k <= 2n * T...
 * Range:     u8 n <= 63, u16 n <= 195, u32 n <= 675, u64 n <= 2303,
 *            u128 n <= 8648, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
 
/* Integer square root (floor) without overflow */
static idx_t int_isqrt(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) <= n / (r + 1))
        ++r;
    return r;
}
 
/* Global memoisation state */
static seq_t **rows = NULL;   /* array of pointers to rows */
static idx_t max_row = (idx_t)-1; /* highest computed row */
static size_t rows_cap = 0;   /* capacity of rows array */
 
/* Ensure base rows 0 and 1 are allocated */
static void ensure_base(void)
{
    if (max_row != (idx_t)-1)
        return;   /* already initialised */
 
    rows_cap = 2;
    rows = (seq_t**)realloc(rows, rows_cap * sizeof(seq_t*));
 
    /* Row 0: length 1 */
    seq_t *row0 = (seq_t*)malloc(sizeof(seq_t));
    row0[0] = (seq_t)1;
    rows[0] = row0;
 
    /* Row 1: length 3 */
    seq_t *row1 = (seq_t*)malloc(3 * sizeof(seq_t));
    row1[0] = (seq_t)0;
    row1[1] = (seq_t)0;
    row1[2] = (seq_t)1;
    rows[1] = row1;
 
    max_row = 1;
}
 
/* Compute rows up to target_row (inclusive) */
static void compute_up_to(idx_t target_row)
{
    if (target_row <= max_row)
        return;   /* already computed */
 
    if (rows_cap < (size_t)(target_row + 1)) {
        rows_cap = (size_t)(target_row + 1);
        rows = (seq_t**)realloc(rows, rows_cap * sizeof(seq_t*));
    }
 
    for (idx_t n = max_row + 1; n <= target_row; ++n) {
        size_t len = 2 * (size_t)n + 1;
        seq_t *row = (seq_t*)malloc(len * sizeof(seq_t));
        for (size_t i = 0; i < len; ++i)
            row[i] = (seq_t)0;
 
        if (n == 0) {
            row[0] = (seq_t)1;
        }
        else if (n == 1) {
            /* already zero except row[2] which is 1 */
            row[0] = (seq_t)0;
            row[1] = (seq_t)0;
            row[2] = (seq_t)1;
        }
        else {  /* n >= 2 */
            /* T(n,0) = 2*T(n-2,1)+2*T(n-1,2) */
            seq_t t_n2_1 = 0, t_n1_2 = 0;
            if (1 <= 2 * (n - 2))
                t_n2_1 = rows[n - 2][1];
            if (2 <= 2 * (n - 1))
                t_n1_2 = rows[n - 1][2];
            row[0] = 2 * t_n2_1 + 2 * t_n1_2;
 
            /* T(n,1) = T(n-2,0)+T(n-2,2)+T(n-1,3)+T(n-1,1) */
            seq_t t_n2_0 = rows[n - 2][0];  /* exists */
            seq_t t_n2_2 = 0, t_n1_3 = 0, t_n1_1 = 0;
            if (2 <= 2 * (n - 2))
                t_n2_2 = rows[n - 2][2];
            if (3 <= 2 * (n - 1))
                t_n1_3 = rows[n - 1][3];
            if (1 <= 2 * (n - 1))
                t_n1_1 = rows[n - 1][1];
            row[1] = t_n2_0 + t_n2_2 + t_n1_3 + t_n1_1;
 
            /* General recurrence for 2 <= k <= 2n */
            for (idx_t k = 2; k <= 2 * n; ++k) {
                seq_t sum = 0;
                if (k - 1 <= 2 * (n - 2))
                    sum += rows[n - 2][k - 1];
                if (k + 1 <= 2 * (n - 2))
                    sum += rows[n - 2][k + 1];
                if (k - 2 <= 2 * (n - 1))
                    sum += rows[n - 1][k - 2];
                if (k + 2 <= 2 * (n - 1))
                    sum += rows[n - 1][k + 2];
                row[(size_t)k] = sum;
            }
        }
 
        rows[n] = row;
    }
 
    max_row = target_row;
}
 
/* Public sequence function */
seq_t a(idx_t n)
{
    /* Map flat index n to row and offset */
    idx_t row = int_isqrt(n);
    idx_t offset = n - row * row;
 
    ensure_base();
    compute_up_to(row);
 
    return rows[row][(size_t)offset];
}
