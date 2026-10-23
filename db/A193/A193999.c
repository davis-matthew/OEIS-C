/*
 * A193999  Mirror of the triangle A094585.
 * https://oeis.org/A193999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 231, u32 n <= 990, u64 n <= 4095,
 *            u128 n <= 9966, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula
 * -------
 * Let r be the smallest integer such that
 *      r * (r + 1) / 2 ≥ n                                (1)
 * Let p = n - r * (r - 1) / 2.  Then 1 ≤ p ≤ r.
 *
 * The j‑th element of row r of A094585 is
 *     w(r,p) = Fib(r + 3) – Fib(p + 2)
 *
 * A193999 is obtained by listing the rows of A094585 in the
 * natural order, so
 *
 *     a(n) = Fib(r + 3) – Fib(p + 2)                       (2)
 *
 * where r and p satisfy (1) and p = n – r(r‑1)/2.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0)      /* the sequence has offset 1, the value for n=0 is unused */
        return 0;

    /* --- find the row r such that r*(r+1)/2 >= n ---------------------- */
    idx_t r = 0;
    while ((idx_t)((__int128)r * (r + 1) / 2) < n)
        ++r;

    /* --- position in the row ------------------------------------------ */
    idx_t triangular_prev = (idx_t)((__int128)(r - 1) * r / 2);
    idx_t p = n - triangular_prev;          /* 1‑based column index */

    /* --- compute Fibonacci terms ------------------------------------- */
    seq_t fib_r = fib(r + 3);        /* Fib(r+3)  */
    seq_t fib_c = fib(p + 2);        /* Fib(p+2)  */

    /* --- return the value of the triangle entry ---------------------- */
    return fib_r - fib_c;
}
