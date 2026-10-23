/*
 * A128089  Denominators in inverse of triangle A128078 by rows, n * each term
 *          in n-th row of A126615.
 * https://oeis.org/A128089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, frac
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   gcd, offset_before_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A128089: denominators in inverse of triangle A128078.
    The triangle A128078 has entries
          T(i,j) = 1/j   (i>j)   and   T(i,i)=i^2.
    Let L be this lower‑triangular matrix and L⁻¹ its inverse.
    The value a(n) is the denominator of L⁻¹(i,j) in lowest terms,
    where (i,j) is the nth element read row‑by‑row (n ≥ 1).
    Recurrence used:
          y(i,i) = 1 / (i²)
          y(i,j) = (1 / i²) * Σ_{k=j}^{i-1} k * y(k,j)
    The algorithm builds the required entry by iterating from the
    diagonal upwards, keeping the sum S = Σ_{m=j}^{k-1} m*y(m,j)
    as a rational number (numerator/denominator).  All arithmetic is
    performed on the integer type seq_t and uses only the usual
    operators and the gcd function.  This is valid for both C11 and
    C++17 builds, regardless of seq_t being an 8/16/32/64‑bit unsigned
    integer or a big‑integer class. */
#if 0
/* This flag is only here to keep the code block tidy; it is not
   part of the program. */
#endif

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/offset_before_row.h"

/* Return the row (1‑based) that contains the nth element. */
static idx_t
row_for_index(idx_t n)
{
    idx_t lo = 1, hi = n;               /* hi ≥ answer */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        idx_t h = mid * (mid + 1) / 2;
        if (h >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Reduce a rational number represented by num/den. */
static void
reduce(seq_t *num, seq_t *den)
{
    seq_t g = gcd(*num, *den);
    *num /= g;
    *den /= g;
}

/* Add a fraction a/b to the current rational *num / *den. */
static void
add_to_rational(seq_t *num, seq_t *den, seq_t a, seq_t b)
{
    seq_t new_num = (*num) * b + a * (*den);
    seq_t new_den = (*den) * b;
    reduce(&new_num, &new_den);
    *num = new_num;
    *den = new_den;
}

/* Compute denominator of L⁻¹(i,j).  i,j are 1‑based, i≥j. */
static seq_t
denominator_row_col(idx_t i, idx_t j)
{
    if (j == i)        /* diagonal */
        return (seq_t)i * (seq_t)i;   /* denominator of 1/(i²) */

    seq_t S_num = 1;                /* S = Σ_{m=j}^{k-1} m*y(m,j)   */
    seq_t S_den = (seq_t)j;          /* start with j*y(j,j)=1/j */

    for (idx_t k = j + 1; k <= i; ++k) {
        seq_t ksq = (seq_t)k * (seq_t)k;
        /* y_k = S / (k²)  (using current S) */
        seq_t y_num = S_num;
        seq_t y_den = S_den * ksq;
        reduce(&y_num, &y_den);

        if (k == i)            /* last step: result* */
            return y_den;

        /* add k*y_k to S */
        seq_t my_num = y_num * (seq_t)k;  /* k * y_num */
        seq_t my_den = y_den;
        add_to_rational(&S_num, &S_den, my_num, my_den);
    }
    /* Should never reach here */
    return 0;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    idx_t i = row_for_index(n);
    idx_t j = n - offset_before_row(i);
    return denominator_row_col(i, j);
}
