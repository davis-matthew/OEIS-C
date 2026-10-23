/*
 * A155994  A triangle of polynomial coefficients: p(x,n)=-(ChebyshevU[n, x] -
 *          ((x + 1)^n - (1 - x)^n)); sp(x,n) = p(x, n) + x^n*p(1/x, n).
 * https://oeis.org/A155994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, uned
 * Formula:   a(n) is the n‑th element of the flattened triangle sp(x,n) =
 *            p(x,n) + x^n p(1/x,n), where p(x,n) = -(ChebyshevU[n,x] -
 *            ((x+1)^n - (1-x)^n)). The triangle entries are read row by row
 *            (row 0 has one element, row 1 is empty). a(0) = -2, a(1) = -3,
 *            a(2)=8,
 * Range:     i8 n <= 27, i16 n <= 56, i32 n <= 56, i64 n <= 56, i128 n <= 56,
 *            big any n
 * Helpers:   binom, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- helpers from the build system ---------- */
#include <stdint.h>
#include "../utils/binom.h"   /* seq_t binom(idx_t a, idx_t b); */
#include "../utils/mul_small.h" /* seq_t mul_small(seq_t x, unsigned int m); */

/* ---------- local utility functions ---------- */

/* integer power of 2: 2^exp   (exp may be large) */
static inline seq_t pow_two(idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res = res * 2;
    return res;
}

/* coefficient of U_n(x) at power x^j
   U_n(x) = sum_{k=0..⌊n/2⌋} (-1)^k binom(n-k,k) (2x)^{n-2k}   */
static inline seq_t coeff_U(idx_t n, idx_t j)
{
    /* U_n contains only powers of same parity as n */
    if ( ((n - j) & 1) != 0 )
        return 0;

    idx_t k = (n - j) >> 1;          /* (n-j)/2   */
    idx_t a = (n + j) >> 1;          /* (n+j)/2   */

    seq_t bin = binom(a, k);         /* binom((n+j)/2, (n-j)/2) */
    seq_t pow2 = pow_two(j);         /* 2^j  */
    seq_t coeff = bin * pow2;

    if ( k & 1 )
        coeff = -coeff;              /* (-1)^k factor */
    return coeff;
}

/* coefficient of p(x,n) at power x^j   */
static inline seq_t coeff_p(idx_t n, idx_t j)
{
    seq_t u = coeff_U(n, j);                 /* U term */
    seq_t r = -u;                            /* -U_n part */

    if ( j & 1 ) {                           /* odd j adds 2*binom(n,j) */
        seq_t odd = binom(n, j);
        odd = odd * 2;                       /* multiply by 2 */
        r = r + odd;
    }
    return r;
}

/* coefficient of sp(x,n) at power x^j */
static inline seq_t coeff_sp(idx_t n, idx_t j)
{
    return coeff_p(n, j) + coeff_p(n, n - j);
}

/* cumulative count of elements up to row r (r >= 0):
   row 0 has 1 element, row 1 has 0 elements, rows r>=2 have r+1 elements.
   Number of elements before row r is:
        C(r) = r*(r+3)/2 - 1   (valid for r >= 1)
   (r=0 gives C(0)=0, r=1 gives C(1)=1, r=2 gives C(2)=4, etc.) */
static inline idx_t cumulative(idx_t r)
{
    /* use 128‑bit intermediate to avoid overflow in formula */
    unsigned __int128 t = (unsigned __int128)r * (r + 3);
    t = t / 2 - 1;
    return (idx_t)t;
}

/* ---------- public function ---------- */

seq_t a(idx_t idx)
{
    if (idx == 0)                /* row 0 */
        return (seq_t)-2;

    /* find the triangle row r (r >= 2) that contains the idx‑th element */
    /* binary search on the cumulative count */
    idx_t low = 2, high = 2;
    while (cumulative(high) <= idx) {
        if (high > (UINT64_MAX >> 1))
            high = UINT64_MAX;
        else
            high <<= 1;
    }

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (cumulative(mid) <= idx)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t row = low;

    /* previous cumulative count (elements before this row) */
    idx_t prev_cum = cumulative(row - 1);

    /* index within the row (0‑based) */
    idx_t k = idx - prev_cum;

    /* compute coefficient */
    return coeff_sp(row, k);
}
