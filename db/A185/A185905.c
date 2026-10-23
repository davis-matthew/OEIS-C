/*
 * A185905  Rectangular array binomial(k+3,4)*binomial(n+3,4), by
 *          antidiagonals.
 * https://oeis.org/A185905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(k+3,4) * binomial(m+3,4) where (k,m) are
 *            obtained from the antidiagonal enumeration of the array T(k,m) =
 *            binomial(k+3,4)*binomial(m+3,4)
 * Range:     u8 n <= 16, u16 n <= 97, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Compute C(n,4) = n*(n-1)*(n-2)*(n-3)/24 for n ≥ 4. */
static inline seq_t binom4(idx_t n)
{
    seq_t a = (seq_t)n;
    seq_t b = (seq_t)(n-1);
    seq_t c = (seq_t)(n-2);
    seq_t d = (seq_t)(n-3);
    return a * b * c * d / (seq_t)24;
}

seq_t a(idx_t n)
{
    /* The sequence is only defined for n ≥ 1. */
    if (n == 0)
        return 0;

    /* Find the diagonal S (k + m) that contains the n‑th term. */
    idx_t S   = 2;      /* diagonal index (sum of coordinates) */
    idx_t acc = 1;      /* cumulative count of terms up to previous diagonal */

    while (acc < n) {
        ++S;
        acc += S - 1;   /* every new diagonal adds (S‑1) terms */
    }

    idx_t prev = acc - (S - 1);   /* terms before the current diagonal */
    idx_t k    = n - prev;        /* 1‑based column index along the diagonal */
    idx_t m    = S - k;           /* 1‑based row index */

    seq_t tb = binom4(k + 3);      /* binomial(k+3,4) */
    seq_t rb = binom4(m + 3);      /* binomial(m+3,4) */

    return tb * rb;
}
