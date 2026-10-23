/*
 * A181974  Triangle T(n,k), read by rows, given by (1, 1, -1, 0, 0, 0, 0, 0,
 *          0, 0, ...) DELTA (1, 0, -3, 2, 0, 0, 0, 0, 0, 0, 0, ...) where
 *          DELTA is the operator defined in A084938.
 * https://oeis.org/A181974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the flattened triangle T(n,k) where T(0,0) = 1 T(1,0) =
 *            T(1,1) = 1 T(2,0) = 2, T(2,1) = 3, T(2,2) = 1 and for n ≥ 3, k ≥
 *            0: T(n,k) = T(n-1,k) + T(n-2,k) + T(n-2,k-2) with the convention
 *            T(n,k)=0 when k<0 or k>n. a(n) = T(r, p) where r is the row such
 *            that n = 0 + 1 + … + r + p with 0 ≤ p ≤ r
 * Range:     u8 n <= 57, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for the triangle rows */
static seq_t **T_rows   = NULL;   /* array of pointers to rows   */
static size_t   T_cap   = 0;      /* allocated capacity of T_rows */
static size_t   T_count = 0;      /* number of rows actually stored */

/* static storage for Fibonacci numbers needed for T(n,0) */
static seq_t *fib      = NULL;    /* array of Fibonacci numbers    */
static size_t  fib_cap  = 0;      /* allocated capacity of fib     */
static size_t  fib_cnt  = 0;      /* number of Fibonacci numbers stored */

/* ensure fib[n] is available (fib[0]=0, fib[1]=1) */
static void ensure_fib(idx_t n)
{
    if (fib_cnt == 0) {
        fib_cap = 2;
        fib_cnt = 2;
        fib = (seq_t *)malloc(fib_cap * sizeof(seq_t));
        fib[0] = 0;
        fib[1] = 1;
    }
    while (fib_cnt <= (size_t)n) {
        seq_t next = fib[fib_cnt - 1] + fib[fib_cnt - 2];
        if (fib_cnt == fib_cap) {
            size_t new_cap = fib_cap * 2;
            seq_t *tmp = (seq_t *)realloc(fib, new_cap * sizeof(seq_t));
            fib = tmp;
            fib_cap = new_cap;
        }
        fib[fib_cnt] = next;
        fib_cnt++;
    }
}

/* build all triangle rows up to (and including) target */
static void ensure_rows(idx_t target)
{
    while (T_count <= (size_t)target) {
        idx_t      r    = (idx_t)T_count;
        size_t     len  = (size_t)r + 1;
        seq_t *    row  = (seq_t *)malloc(len * sizeof(seq_t));

        if (r == 0) {
            row[0] = 1;
        } else if (r == 1) {
            row[0] = 1;
            row[1] = 1;
        } else if (r == 2) {
            row[0] = 2;
            row[1] = 3;
            row[2] = 1;
        } else {
            /* T(r,0) = Fibonacci(r+1) */
            ensure_fib(r + 1);
            row[0] = fib[r + 1];
            for (idx_t k = 1; k <= r; ++k) {
                seq_t val = 0;
                if (k <= r - 1)  val += T_rows[r - 1][k];
                if (k <= r - 2)  val += T_rows[r - 2][k];
                if (k >= 2)      val += T_rows[r - 2][k - 2];
                row[k] = val;
            }
        }

        if (T_count == T_cap) {
            size_t new_cap = T_cap ? T_cap * 2 : 1;
            seq_t **tmp = (seq_t **)realloc(T_rows, new_cap * sizeof(seq_t *));
            T_rows = tmp;
            T_cap  = new_cap;
        }
        T_rows[T_count] = row;
        T_count++;
    }
}

/* find the row r that contains the flattened index n */
static idx_t row_of(idx_t n)
{
    idx_t r = 0;
    /* use 128‑bit arithmetic to avoid overflow for n up to 64bits */
    while (((unsigned __int128)r * (r + 1)) / 2 <= n)
        ++r;
    return r - 1;
}

/* main function requested by the problem statement */
seq_t a(idx_t n)
{
    idx_t r   = row_of(n);
    idx_t pos = n - (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
    ensure_rows(r);
    return T_rows[r][pos];
}
