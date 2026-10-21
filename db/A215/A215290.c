/*
 * A215290  Number of permutations of 0..floor((n*6-1)/2) on even squares of
 *          an nX6 array such that each row and column of even squares is
 *          increasing.
 * https://oeis.org/A215290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A060854(3, floor((n+1)/2)) * A060854(3, floor(n/2)) *
 *            binomial(3*n, 3*floor((n+1)/2)) where A060854(3,m) = number of
 *            standard Young tableaux of the 3×m rectangle = 2·(3m)! /
 *            ((m+2)!(m+1)!m!)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/binomial.h"

static seq_t SYT_3(idx_t m) {
    /* 2 * factorial(3*m) / (factorial(m+2) * factorial(m+1) * factorial(m)) */
    seq_t num = factorial(3 * m);
    seq_t den = factorial(m + 2);
    den = den * factorial(m + 1);
    den = den * factorial(m);
    num = num * 2;
    return num / den;
}

extern seq_t a(idx_t n) {
    idx_t f3 = (n + 1) / 2;   /* floor((n+1)/2) */
    idx_t f4 = n / 2;         /* floor(n/2)     */

    seq_t t1 = SYT_3(f3);
    seq_t t2 = SYT_3(f4);
    seq_t t3 = binomial(3 * n, 3 * f3);

    return t1 * t2 * t3;
}
