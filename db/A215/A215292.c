/*
 * A215292  T(n,k)=Number of permutations of 0..floor((n*k-1)/2) on even
 *          squares of an nXk array such that each row and column of even
 *          squares is increasing.
 * https://oeis.org/A215292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 40, u32 n <= 71, u64 n <= 126,
 *            u128 n <= 218, big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A215292
 *  a(n) = T(row, col)
 *  where T(n,k) = A060854(f1,f3) * A060854(f2,f4) * binomial(f1*f3+f2*f4, f1*f3)
 *  f1 = floor((k+1)/2), f2 = floor(k/2), f3 = floor((n+1)/2), f4 = floor(n/2).
 *
 *  The b-file lists the triangle in anti‑diagonal order:
 *    d = row + col
 *    for d = 2,3,4,...
 *      for row = 1 .. d-1
 *        col = d - row
 *        output T(row,col)
 *  Thus a(1)=T(1,1), a(2)=T(1,2), a(3)=T(2,1), a(4)=T(1,3), a(5)=T(2,2), ...
 */

#include <stdint.h>
#include <stdbool.h>

/* utility functions provided by the build environment */
#include "../utils/factorial.h"   /* seq_t factorial(idx_t); */
#include "../utils/binomial.h"    /* seq_t binomial(idx_t, idx_t); */

/* helper that computes the binomial coefficient A060854(a,b)
 * (number of standard Young tableaux of rectangular shape a×b)
 */
static seq_t A060854(idx_t a, idx_t b)
{
    if (a == 0 || b == 0) return 1;
    idx_t cells = a * b;
    seq_t res = factorial(cells);          /* (a*b)!   */
    for (idx_t i = 0; i < a; ++i) {
        for (idx_t j = 0; j < b; ++j) {
            seq_t hook = (seq_t)(a - i + b - j - 1);   /* hook length */
            res /= hook;
        }
    }
    return res;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* find the diagonal d such that (d-1)*d/2 >= n */
    idx_t d = 2;
    idx_t cum = 1;  /* cum = (d-1)d/2  when d==2 */
    while (cum < n) {
        ++d;
        cum = (d - 1) * d / 2;
    }

    idx_t prev = (d - 2) * (d - 1) / 2;        /* terms before this diagonal */
    idx_t offset = n - prev;                   /* 1‑based index inside diagonal */
    idx_t row = offset;                        /* n in table */
    idx_t col = d - offset;                    /* k in table */

    /* compute f1..f4 */
    const idx_t f1 = (col + 1) / 2;
    const idx_t f2 = col / 2;
    const idx_t f3 = (row + 1) / 2;
    const idx_t f4 = row / 2;

    /* compute the product A060854(f1,f3) * A060854(f2,f4) * binomial(…) */
    seq_t part1 = A060854(f1, f3);
    seq_t part2 = A060854(f2, f4);
    const idx_t total12 = f1 * f3 + f2 * f4;
    const idx_t choose_k = f1 * f3;
    seq_t part3 = binomial(total12, choose_k);

    return part1 * part2 * part3;
}
