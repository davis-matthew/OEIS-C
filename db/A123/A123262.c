/*
 * A123262  Fibonacci-tribonacci triangle.
 * https://oeis.org/A123262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) – the N⁴th term of the Fibonacci‑tribonacci triangle.
 * It is computed by generating the triangle entries with the recurrence
 *
 *   T(n,k) = T(n‑1,k‑1) + T(n‑2,k‑2) + T(n‑3,k‑2)   (k>0)
 *
 * with boundary conditions
 *
 *   T(n,0) = 0,     T(1,1) = 1,
 *   T(n,k) = 0 for k<0 or k>n,
 *   T(n,n) = Fibonacci(n).
 *
 * The sequence a(n) is obtained by reading the triangle row‑by‑row
 * (row 0: 0; row 1: 0, 1; row 2: 0, 0, 1; …) and enumerating the
 * entries.  This function returns the exact value for any n ≥ 0,
 * independently of the order of calls.
 */

#include <stdlib.h>
#include <stdint.h>

/* static storage for the triangle rows (row_ptrs[i] points to row i)   */
static seq_t **row_ptrs   = NULL;   /* ptr to array of pointers to rows   */
static idx_t   rows_allocated = 0;  /* capacity of row_ptrs array         */
static idx_t   rows_current   = 0;  /* number of rows actually generated   */

/* static storage for Fibonacci numbers (fib_arr[i] = Fib(i))            */
static seq_t *fib_arr = NULL;       /* array of Fibonacci numbers         */
static idx_t   fib_len   = 0;       /* number of Fibonacci numbers stored */

/* Ensure row_ptrs can hold indices up to r (inclusive).                */
static void
ensure_rows_capacity(idx_t r)
{
    if (rows_allocated <= r) {
        size_t new_cap = (size_t)(r + 1);
        seq_t **tmp = (seq_t **)realloc(row_ptrs, new_cap * sizeof(seq_t *));
        row_ptrs = tmp;
        rows_allocated = new_cap;
    }
}

/* Ensure Fibonacci numbers up to index r (inclusive).                  */
static void
ensure_fib_up_to(idx_t r)
{
    if (fib_len == 0) {                /* first time: initialise 0, 1       */
        fib_arr = (seq_t *)malloc(2 * sizeof(seq_t));
        fib_arr[0] = (seq_t)0;
        fib_arr[1] = (seq_t)1;
        fib_len   = 2;
    }
    while (fib_len <= r) {
        seq_t next = fib_arr[fib_len - 1] + fib_arr[fib_len - 2];
        seq_t *tmp = (seq_t *)realloc(fib_arr,
                                      (size_t)(fib_len + 1) * sizeof(seq_t));
        fib_arr   = tmp;
        fib_arr[fib_len] = next;
        ++fib_len;
    }
}

/* Generate triangle rows up to row r (inclusive).                     */
static void
ensure_rows_up_to(idx_t r)
{
    if (rows_current > 0 && r < rows_current - 1)
        return;                     /* already have them                */

    ensure_rows_capacity(r);

    for (idx_t i = rows_current; i <= r; ++i) {
        seq_t *row = (seq_t *)malloc((size_t)(i + 1) * sizeof(seq_t));

        for (idx_t k = 0; k <= i; ++k) {
            if (k == 0) {
                row[k] = (seq_t)0;
            } else if (k == i) {           /* diagonal – Fibonacci number */
                ensure_fib_up_to(i);
                row[k] = fib_arr[i];
            } else {
                seq_t val = row_ptrs[i - 1][k - 1];
                if (k >= 2) {
                    val += row_ptrs[i - 2][k - 2];
                    if (i >= 3)
                        val += row_ptrs[i - 3][k - 2];
                }
                row[k] = val;
            }
        }

        row_ptrs[i] = row;
    }

    rows_current = r + 1;
}

/* Main sequence function.                                            */
seq_t a(idx_t n)
{
    /* Find the row r that contains element n.                         */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Compute the position k within that row (starting from 0).       */
    idx_t offset = r * (r + 1) / 2;   /* start index of row r          */
    idx_t k = n - offset;

    /* Generate required rows and Fibonacci numbers.                  */
    ensure_rows_up_to(r);

    /* Return the requested entry.                                    */
    return row_ptrs[r][k];
}
