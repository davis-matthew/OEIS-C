/*
 * A052174  Triangle of numbers arising in enumeration of walks on square
 *          lattice.
 * https://oeis.org/A052174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A052174 – Triangle of numbers arising in enumeration of walks on square lattice.
    The sequence is obtained by listing the triangle T(n,k) (n≥0, 0≤k≤n) row by
    row.  The triangle entry satisfies

        let d = n – k ,  m = ⌊d/2⌋
        if d is even:
              T(n,k) = C(n+1,m)·C(n,m) – C(n+1,m)·C(n,m‑1)
        if d is odd:
              T(n,k) = C(n+1,m)·C(n,m+1) – C(n+1,m+1)·C(n,m‑1)

    (C(a,b)=0 when b<0 or b>a.)  The function below implements this
    formula and returns the n-th term of the flattened sequence, with
    a(0)=19.  The function is fully self‑contained except for the
    provided binomial routine `binom()`.  No lambda, VLA or dynamic
    allocation is used. */

#include <stdint.h>
#include "../utils/binom.h"

/* Return binomial coefficient C(a,b); 0 when b>a. */
static inline seq_t binom_safe(idx_t a, idx_t b)
{
    if (b > a) return 0;
    return binom(a, b);
}

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column k in this row. */
    idx_t k = n - r * (r + 1) / 2;

    /* Differences for the formula. */
    idx_t d = r - k;          /* d = n - y with y = k */
    idx_t m = d / 2;          /* floor(d/2) */

    /* Compute the required binomial terms, guarding out‑of‑range. */
    seq_t c1 = binom_safe(r + 1, m);
    seq_t c3;
    seq_t c2, c4;

    if (d & 1) {                            /* d is odd */
        if (m + 1 <= r)
            c2 = binom_safe(r, m + 1);
        else
            c2 = 0;

        c3 = binom_safe(r + 1, m + 1);
        c4 = (m > 0) ? binom_safe(r, m - 1) : 0;
    } else {                                 /* d is even */
        c2 = binom_safe(r, m);
        c3 = 0;                                /* not used */
        c4 = (m > 0) ? binom_safe(r, m - 1) : 0;
    }

    /* Evaluate the formula. */
    seq_t result;
    if (d & 1) {                            /* odd case */
        result = c1 * c2 - c3 * c4;
    } else {                                 /* even case */
        result = c1 * c2 - c1 * c4;
    }

    return result;
}
