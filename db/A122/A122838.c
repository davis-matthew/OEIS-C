/*
 * A122838  Triangular array read by rows: row n gives coefficients of
 *          p(n)(x), where p(0)=1, p(1)=x-1, p(n) =
 *          (x-fibonacci(n))*p(n-1)-(n-1)*p(n-2).
 * https://oeis.org/A122838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) is the flattened triangular array of coefficients of
 *            polynomials p(n)(x) defined by p(0)=1, p(1)=x-1, p(n) = (x -
 *            fib(n)) * p(n-1) - (n-1) * p(n-2) (n ≥ 2) The coefficients are
 *            read row by row, where row r contains r+1 coefficients
 * Range:     i8 n <= 20, i16 n <= 35, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: fib(n) is the standard Fibonacci sequence with fib(0)=0,
   fib(1)=1.  The function is memoized; it works for any order of
   calls and is O(n) in the required row size. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static state for memoization */
    static seq_t **polys = NULL;            /* p(r) coefficient arrays */
    static size_t polys_cap = 0;            /* capacity of polys array */
    static idx_t polys_max = 0;             /* highest r computed so far */

    static seq_t *fibs = NULL;             /* Fibonacci numbers */
    static size_t fibs_cap = 0;             /* capacity of fibs array */
    static idx_t fibs_max = 0;              /* highest index of fibs computed */

    /* Find the row r that contains index n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    /* Initial allocation on first call */
    if (polys == NULL) {
        polys_cap = 2;
        polys = (seq_t **)malloc(polys_cap * sizeof(seq_t *));
        polys[0] = (seq_t *)malloc(1 * sizeof(seq_t));
        polys[0][0] = (seq_t)1;            /* p(0) = 1 */
        polys[1] = (seq_t *)malloc(2 * sizeof(seq_t));
        polys[1][0] = (seq_t)(-1);          /* p(1) = x - 1 */
        polys[1][1] = (seq_t)1;
        polys_max = 1;

        fibs_cap = 2;
        fibs = (seq_t *)malloc(fibs_cap * sizeof(seq_t));
        fibs[0] = (seq_t)0;                 /* fib(0) = 0 */
        fibs[1] = (seq_t)1;                 /* fib(1) = 1 */
        fibs_max = 1;
    }

    /* Ensure we have computed all rows up to r */
    if (r > polys_max) {

        /* Extend polys capacity if needed */
        if (polys_cap < (size_t)(r + 1)) {
            size_t new_cap = polys_cap ? polys_cap * 2 : 2;
            if (new_cap < (size_t)(r + 1))
                new_cap = r + 1;
            seq_t **tmp = (seq_t **)realloc(polys, new_cap * sizeof(seq_t *));
            polys = tmp;
            polys_cap = new_cap;
        }

        /* Extend fibs capacity if needed */
        if (fibs_cap < (size_t)(r + 1)) {
            size_t new_cap = fibs_cap ? fibs_cap * 2 : 2;
            if (new_cap < (size_t)(r + 1))
                new_cap = r + 1;
            seq_t *tmp = (seq_t *)realloc(fibs, new_cap * sizeof(seq_t));
            fibs = tmp;
            fibs_cap = new_cap;
        }

        /* Compute missing Fibonacci numbers */
        for (idx_t i = fibs_max + 1; i <= r; ++i) {
            fibs[i] = fibs[i - 1] + fibs[i - 2];
        }
        fibs_max = r;

        /* Compute missing polynomial rows */
        for (idx_t i = polys_max + 1; i <= r; ++i) {
            polys[i] = (seq_t *)malloc((size_t)(i + 1) * sizeof(seq_t));
            for (idx_t k = 0; k <= i; ++k) {
                seq_t t1 = (k > 0)                    ? polys[i - 1][k - 1] : (seq_t)0;
                seq_t t2 = (k < i)                    ? polys[i - 1][k] * fibs[i] : (seq_t)0;
                seq_t t3 = (k <= i - 2)               ? (seq_t)(i - 1) * polys[i - 2][k] : (seq_t)0;
                polys[i][k] = t1 - t2 - t3;
            }
        }
        polys_max = r;
    }

    /* Retrieve coefficient at position j in row r */
    idx_t offset = r * (r + 1) / 2;
    idx_t j = n - offset;
    return polys[r][j];
}
