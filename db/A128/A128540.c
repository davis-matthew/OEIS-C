/*
 * A128540  Triangle A127647 * A097806, read by rows.
 * https://oeis.org/A128540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the nth element of the lower‑triangular matrix *
 *            T(r,c)=Fib(r) if c==r or c==r-1, otherwise 0. * Here Fib(1)=1,
 *            Fib(2)=1. The sequence is read row‑wise
 * Range:     u8 n <= 103, u16 n <= 323, u32 n <= 1174, u64 n <= 4463,
 *            u128 n <= 5050, big any n
 * Validated: standard, pass, 94 terms through n = 4987 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    idx_t t = 1;              /* total entries up to row r */
    while (t < n) {
        ++r;
        t += r;
    }
    /* Column index within the row */
    idx_t col = n - (t - r);
    if (col == r || col == r - 1) {
        return fib(r);
    }
    return 0;
}
