/*
 * A154851  Symmetrical triangular sequence of Fibonacci numbers (A000045):
 *          p(x,n) = Product[1 + Fibonacci[i]*x, {i, 0, n}] + x^n*Product[1 +
 *          Fibonacci[i]/x, {i, 0, n}].
 * https://oeis.org/A154851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   a(n) = e_m + e_{r-m} where r is the unique row with (r+1)(r+2)/2
 *            > n, m = n - r(r+1)/2, e_j are elementary symmetric sums of the
 *            Fibonacci numbers F[0..r] (F[0]=0, F[1]=1)
 * Range:     u8 n <= 21, u16 n <= 36, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for realloc */

seq_t a(idx_t n)
{
    /* locate the row r and column m for the flat index n */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;
    idx_t base = r * (r + 1) / 2;
    idx_t m = n - base;

    /* compute Fibonacci numbers up to F[r] */
    static seq_t *fib = NULL;
    static size_t fib_size = 0;
    if ((size_t)(r + 1) > fib_size) {
        fib_size = (size_t)(r + 1);
        fib = (seq_t *)realloc(fib, fib_size * sizeof(seq_t));
    }
    fib[0] = (seq_t)0;
    if (r >= 1) fib[1] = (seq_t)1;
    for (idx_t i = 2; i <= r; ++i)
        fib[i] = fib[i - 1] + fib[i - 2];

    /* compute elementary symmetric sums e[0..r] for F[0..r] */
    static seq_t *e = NULL;
    static size_t e_size = 0;
    if ((size_t)(r + 1) > e_size) {
        e_size = (size_t)(r + 1);
        e = (seq_t *)realloc(e, e_size * sizeof(seq_t));
    }
    e[0] = (seq_t)1;
    for (idx_t j = 1; j <= r; ++j)
        e[j] = (seq_t)0;

    for (idx_t i = 0; i <= r; ++i) {
        for (idx_t j = i; j >= 1; --j) {
            e[j] = e[j] + e[j - 1] * fib[i];
        }
    }

    /* symmetric coefficient */
    return e[m] + e[r - m];
}
