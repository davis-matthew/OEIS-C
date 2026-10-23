/*
 * A158950  Triangle read by rows, A158948 * (an infinite lower triangular
 *          matrix with A000071 prefaced with a 1 as the right border; and the
 *          rest zeros).
 * https://oeis.org/A158950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  eigen, nonn, tabl
 * Formula:   a(n) = A158948[row(n),col(n)] * (Fib(col(n)+1)-1), with
 *            Fib(0)=0,Fib(1)=1. A158948[row,1] = floor((row+1)/2); for col>1,
 *            A158948[row,col] = 1 if row%2==col%2 else 0
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;

    /* Determine row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) {
        ++r;
    }

    /* Column index c (1‑based) */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t c = n - prev_tri;                  /* 1 <= c <= r */

    /* ---------- Fibonacci table (needed up to c+1) ---------- */
    static seq_t *fib = NULL;
    static size_t fib_cap = 0;   /* capacity in elements, including index 0 */
    static idx_t fib_last = 1;   /* highest index computed, initial Fib[1] */

    idx_t need = c + 1;          /* we need fib[need] */
    if (fib == NULL) {
        fib_cap = (need + 3) * 2;                 /* some room */
        if (fib_cap < 4) fib_cap = 4;
        fib = (seq_t *)malloc(sizeof(seq_t) * fib_cap);
        if (!fib) return (seq_t)0;
        fib[0] = (seq_t)0;
        fib[1] = (seq_t)1;
        fib_last = 1;
    } else if ((size_t)need >= fib_cap) {
        size_t new_cap = fib_cap;
        while ((size_t)need >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(fib, sizeof(seq_t) * new_cap);
        if (!tmp) return (seq_t)0;
        fib = tmp;
        fib_cap = new_cap;
    }

    for (idx_t i = fib_last + 1; i <= need; ++i) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }
    fib_last = need;

    /* ---------- Compute diagonal value diag(c) ---------- */
    seq_t diag;
    if (c == 1) {
        diag = (seq_t)1;
    } else {
        /* diag(c) = Fib(c+1) - 1 */
        diag = fib[c + 1] - (seq_t)1;
    }

    /* ---------- A158948[row][col] ---------- */
    seq_t a158948_val;
    if (c == 1) {
        /* floor((row+1)/2) */
        a158948_val = (seq_t)(r + 1) / 2;
    } else {
        a158948_val = ((r & 1) == (c & 1)) ? (seq_t)1 : (seq_t)0;
    }

    /* Result */
    return a158948_val * diag;
}
