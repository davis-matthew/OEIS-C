/*
 * A085215  Square array A(x,y) = the number whose factorial expansion A007623
 *          is that of x and y concatenated; zero expanded as empty string;
 *          read by ascending antidiagonals: A(0,0), A(1,0), A(0,1), A(2,0),
 *          A(1,1), A(0,2), ...
 * https://oeis.org/A085215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(x,y) where A(x,y) is the number whose factorial *
 *            expansion is the concatenation of the factorial expansions of x
 *            and y. * (x,y) are the antidiagonal coordinates of n obtained
 *            with * antidiagonal_coords(n,&i,&j); the function returns them
 *            as * (i=y, j=x) – i is the y‑coordinate, j is the...
 * Range:     u8 n <= 71, u16 n <= 2651, u32 n <= 22154, u64 n <= 22154,
 *            u128 n <= 22154, big any n
 * Validated: standard, pass, 94 terms through n = 9020 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* mapping from linear index n to pair (i,j) on the antidiagonals of T */
static inline void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* find s such that (s+1)(s+2)/2 <= n < (s+2)(s+3)/2 */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 <= n) ++s;
    idx_t pos = n - (s * (s + 1)) / 2;  /* 0 <= pos <= s */
    *i = pos;
    *j = s - pos;
}

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);   /* i = y, j = x */

    /* factorial digits of x (least significant first) */
    unsigned int digits_x[32];
    unsigned int len_x = 0;
    idx_t tmp = j;
    unsigned int pos = 1;
    while (tmp > 0) {
        digits_x[len_x++] = (unsigned int)(tmp % (pos + 1));
        tmp /= (pos + 1);
        ++pos;
    }

    /* factorial digits of y (least significant first) */
    unsigned int digits_y[32];
    unsigned int len_y = 0;
    tmp = i;
    pos = 1;
    while (tmp > 0) {
        digits_y[len_y++] = (unsigned int)(tmp % (pos + 1));
        tmp /= (pos + 1);
        ++pos;
    }

    /* Result starts with the value of y.  y occupies the low factorial places. */
    seq_t result = (seq_t)i;

    /* factorial corresponding to the first position above y.  */
    seq_t fact = factorial((idx_t)len_y + 1);

    /* Add digits of x shifted to higher factorial positions. */
    for (unsigned int k = 0; k < len_x; ++k) {
        unsigned int d = digits_x[k];
        if (d)
            result += mul_small(fact, d);
        fact = mul_small(fact, (idx_t)len_y + k + 2);
    }

    return result;
}
