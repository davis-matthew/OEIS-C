/*
 * A171145  The sequence of coefficients of a polynomial recursion:
 *          p(x,n)=If[Mod[n, 2] == 0, (x + 1)*p(x, n - 1), (x^2 + n*x +
 *          1)^Floor[n/2]].
 * https://oeis.org/A171145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of x^d in p(x,r) where r = minimal integer
 *            with r*(r+1)/2 >= n, d = n - r*(r-1)/2 - 1. Recurrence:
 *            p(x,0)=1, p(x,r) = (x+1)*p(x,r-1) if r even, p(x,r) = (x^2 + r*x
 *            + 1)^(floor(r/2)) if r odd
 * Range:     u8 n <= 24, u16 n <= 59, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Required external definitions are provided by the framework.  */
#include "../utils/pow_uint.h"  /* seq_t pow_uint(uint64_t base, idx_t exp); */

/* --------- Binomial coefficient (n choose k) -------------------------------- */
static seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* multiply by (n - k + i) and divide by i – exact division */
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* --------- Coefficient for an odd row r (r odd) ----------------------------- */
static seq_t coeff_odd(idx_t r, idx_t d)
{
    /* r is odd, degree d (0 <= d < r) */
    if (d >= r) return 0;          /* out of range */
    idx_t k = r / 2;                /* exponent in definition */
    seq_t sum = 0;
    /* j = number of x^2 terms selected; i = number of n*x terms selected */
    idx_t jmax = (d / 2 < k) ? (d / 2) : k;   /* j <= floor(d/2) and j <= k */
    for (idx_t j = 0; j <= jmax; ++j)
    {
        idx_t i = d - 2 * j;         /* remaining degree comes from n*x terms */
        if (i > k - j) continue;     /* not enough terms left for i x's */
        if (i > k) continue;         /* safety */
        seq_t term = binom_calc(k, j);           /* choose j x^2 terms    */
        term = term * binom_calc(k - j, i);      /* choose i n*x terms   */
        term = term * pow_uint((uint64_t)r, i);   /* weight n^i          */
        sum += term;
    }
    return sum;
}

/* --------- Find row r such that r*(r+1)/2 >= n ------------------------------ */
static idx_t find_row(idx_t n)
{
    idx_t low = 1, high = n;
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        __int128 mid_sum = (__int128)mid * (mid + 1) / 2;
        if (mid_sum >= n) high = mid;
        else              low = mid + 1;
    }
    return low;
}

/* ----------------- Main function required by the framework ----------------- */
seq_t a(idx_t n)
{
    /* Locate the row r and the degree d within that row */
    idx_t r = find_row(n);
    idx_t start = r * (r - 1) / 2 + 1;    /* 1‑based index of first coeff in row r */
    idx_t d = n - start;                  /* 0‑based degree */

    if (r & 1) /* odd row */
    {
        return coeff_odd(r, d);
    }
    else /* even row: use recurrence with previous odd row */
    {
        seq_t val = coeff_odd(r - 1, d);
        if (d > 0) val += coeff_odd(r - 1, d - 1);
        return val;
    }
}
