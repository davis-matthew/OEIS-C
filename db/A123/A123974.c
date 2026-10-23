/*
 * A123974  Fibonacci central tridiagonal matrices as a triangular sequence
 *          from a recursive polynomial definition.
 * https://oeis.org/A123974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, sign
 * Formula:   a(n) is the nth term of the triangular sequence formed by the
 *            polynomials P_k(x) defined by P_0(x) = 1, P_1(x) = x - 1, P_k(x)
 *            = (x - F_k) P_{k-1}(x) - P_{k-2}(x) for k ≥ 2, where F_k is the
 *            k‑th Fibonacci number (F_1 = F_2 = 1). The coefficients of
 *            P_k(x) are listed in ascending order of powers...
 * Range:     i8 n <= 22, i16 n <= 37, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* compute a(n) */
seq_t a(idx_t n)
{
    /* any index < 1 is undefined – return 0 */
    if (n < 1) return 0;

    /* find the row r containing the nth term */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 < n) ++r;            /* r ≥ 1 */

    /* static storage for Fibonacci numbers */
    static seq_t fib[200];
    static idx_t fib_len = 0;

    /* static storage for polynomial coefficients (ascending) */
    static seq_t *polys[200];
    static idx_t polys_len = 0;  /* number of polynomials currently stored */

    /* --- ensure Fibonacci numbers are ready up to index r   --- */
    if (fib_len == 0) {
        fib[0] = (seq_t)0;
        fib[1] = (seq_t)1;
        fib[2] = (seq_t)1;
        fib_len = 3;
    }
    while (fib_len <= r) {
        fib[fib_len] = fib[fib_len - 1] + fib[fib_len - 2];
        ++fib_len;
    }

    /* --- ensure polynomials P_0 … P_{r-1} are computed --- */
    while (polys_len <= r) {
        if (polys_len == 0) {
            /* P_0(x) = 1  -> coefficients: [1] */
            polys[0] = (seq_t*)malloc(sizeof(seq_t) * 1);
            polys[0][0] = (seq_t)1;
        } else if (polys_len == 1) {
            /* P_1(x) = x - 1  -> coefficients: [-1, 1] */
            polys[1] = (seq_t*)malloc(sizeof(seq_t) * 2);
            polys[1][0] = (seq_t)-1;
            polys[1][1] = (seq_t)1;
        } else {
            /* general recurrence */
            idx_t i = polys_len;   /* index of polynomial to compute */
            const seq_t *prev  = polys[i - 1];
            const seq_t *prev2 = polys[i - 2];
            seq_t *cur = (seq_t*)calloc((size_t)(i + 1), sizeof(seq_t));
            /* (x - F_i) * P_{i-1} */
            for (idx_t j = 0; j < i; ++j) {
                cur[j + 1] = prev[j];                  /* x * prev[j]   */
                cur[j] -= fib[i] * prev[j];            /* -F_i * prev[j] */
            }
            /* subtract P_{i-2} */
            for (idx_t j = 0; j < i - 1; ++j) {
                cur[j] -= prev2[j];
            }
            polys[i] = cur;
        }
        ++polys_len;
    }

    /* compute start index of row r */
    idx_t start = ((r - 1) * r) / 2 + 1;
    idx_t offset = n - start;          /* 0‑based offset inside the row */

    seq_t val = polys[r - 1][offset];
    if ((r - 1) & 1) val = -val;        /* alternate sign each row */

    return val;
}
