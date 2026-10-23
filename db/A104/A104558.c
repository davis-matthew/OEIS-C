/*
 * A104558  Triangle, read by rows, equal to the matrix inverse of A104557 and
 *          related to Laguerre polynomials.
 * https://oeis.org/A104558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 40, i16 n <= 97, i32 n <= 264, i64 n <= 722,
 *            i128 n <= 2049, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = T(r,c) where (r,c) are the row and column of index n
 * in the triangle read by rows.
 * T(n,k) = (-1)^(n-k) * (n-k)! *
 *          binom(1+floor(n/2), k+1-floor((n+1)/2)) *
 *          binom(floor((n+1)/2), k-floor(n/2))
 */
 
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom.h"

/* factorial cache for (0,1,2,...)!  */
static seq_t *fact_table = NULL;
static idx_t fact_size   = 0;

/* ensure factorials up to 'up' are in the cache */
static void ensure_factorial(idx_t up)
{
    if (fact_table == NULL) {
        fact_table = (seq_t *)malloc(sizeof(seq_t));
        if (fact_table == NULL) abort();
        fact_table[0] = (seq_t)1;
        fact_size = 1;
    }
    if (up < fact_size - 1) return;

    fact_table = (seq_t *)realloc(fact_table,
                                  sizeof(seq_t) * (up + 1));
    if (fact_table == NULL) abort();

    for (idx_t i = fact_size; i <= up; ++i)
        fact_table[i] = fact_table[i-1] * (seq_t)i;
    fact_size = up + 1;
}

/* integer square‑root: floor(sqrt(x)) */
static inline idx_t int_sqrt(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x)   lo = mid + 1;
        else          hi = mid - 1;
    }
    return hi;
}

/* triangle entry T(n,k) as defined in the formula */
static seq_t triangle_term(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;

    idx_t a = n / 2;        /* floor(n/2)           */
    idx_t b = (n + 1) / 2;  /* floor((n+1)/2)       */

    /* First binomial: C(1+a, k+1-b) */
    idx_t k1 = k + 1 - b;
    if (k1 < 0 || k1 > 1 + a) return (seq_t)0;
    seq_t bin1 = binom(1 + a, k1);

    /* Second binomial: C(b, k-a) */
    idx_t k2 = k - a;
    if (k2 < 0 || k2 > b) return (seq_t)0;
    seq_t bin2 = binom(b, k2);

    /* (n-k)! */
    idx_t fact_nk = n - k;
    ensure_factorial(fact_nk);
    seq_t fact = fact_table[fact_nk];

    /* Combine with sign */
    seq_t res = fact * bin1 * bin2;
    if ((n - k) & 1) res = -res;
    return res;
}

/* public sequence function */
seq_t a(idx_t n)
{
    /* find row r: r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t N = 1 + 8 * n;
    idx_t s = int_sqrt(N);
    idx_t r = (s - 1) / 2;

    /* correct off‑by‑one due to floor_sqrt rounding */
    while (r * (r + 1) / 2 > n) r--;
    while ((r + 1) * (r + 2) / 2 <= n) r++;

    idx_t c = n - r * (r + 1) / 2;
    return triangle_term(r, c);
}
