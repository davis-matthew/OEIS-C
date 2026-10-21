/*
 * A096669  Rectangular array T(n,k) read by antidiagonals; generating
 *          function of row n is 1/F(n,x), where F(n,x) is the polynomial 1 -
 *          x - x^2 - 2*x^3 -...- F(n+1)*x^n and F(n+1) is the (n+1)st
 *          Fibonacci number, for n=0,1,2,...
 * https://oeis.org/A096669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A096669: a(n) is the antidiagonal flattening of a rectangular array T(row,col).
   In each row, T(row,0)=1 and the coefficients satisfy the recurrence
       T(row,k) = Σ_{i=1}^{min(k,row+1)} Fib(i) * T(row,k-i),
   where Fib(1)=Fib(2)=1 and Fib(i)=Fib(i-1)+Fib(i-2). This definition is taken
   directly from the OEIS entry. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset starts at 1 – unused below */

    /* ---------- locate the antidiagonal that contains position n ---------- */

    idx_t s = 0;
    while (1) {
        unsigned __int128 cnt = (unsigned __int128)(s + 1) * (s + 2) / 2;
        if (cnt >= (unsigned __int128)n) break;
        ++s;
    }

    /* number of elements before this diagonal */
    unsigned __int128 prev_cnt = (unsigned __int128)s * (s + 1) / 2;
    idx_t p = (idx_t)((unsigned __int128)n - prev_cnt - 1);   /* 0‑based */

    idx_t r = p;                 /* row index    */
    idx_t c = s - p;             /* column index */

    /* ---------- Fibonacci numbers (needed up to r+1) ---------- */

    /* dynamic storage for Fibonacci numbers; indices start at 1 */
    static seq_t *fib = NULL;
    static idx_t fib_len = 0;          /* highest index computed (>=1) */
    static idx_t fib_cap = 0;

    if (fib == NULL) {
        fib_cap = 16;
        fib = (seq_t *)malloc(fib_cap * sizeof(seq_t));
        fib[0] = 0;               /* unused */
        fib[1] = 1;
        fib[2] = 1;
        fib_len = 2;
    }

    idx_t need = r + 1;         /* need Fibonacci numbers up to Fib(need) */
    while (fib_len < need) {
        /* grow array if necessary */
        if (fib_len + 1 >= fib_cap) {
            fib_cap = fib_cap * 2 > need + 1 ? fib_cap * 2 : need + 1;
            seq_t *tmp = (seq_t *)realloc(fib, fib_cap * sizeof(seq_t));
            if (!tmp) return 0;  /* allocation failure – unlikely */
            fib = tmp;
        }
        ++fib_len;
        fib[fib_len] = fib[fib_len - 1] + fib[fib_len - 2];
    }

    /* ---------- compute row coefficients up to column c ---------- */

    seq_t *row = (seq_t *)malloc((size_t)(c + 1) * sizeof(seq_t));
    if (!row) return 0;               /* allocation failure – unlikely */

    row[0] = 1;                       /* T(r,0) = 1 for all rows */

    for (idx_t k = 1; k <= c; ++k) {
        seq_t sum = 0;
        idx_t max_i = k < r + 1 ? k : r + 1;   /* min(k, r+1) */
        for (idx_t i = 1; i <= max_i; ++i) {
            sum += fib[i] * row[k - i];
        }
        row[k] = sum;
    }

    seq_t result = row[c];
    free(row);
    return result;
}
