/*
 * A080955  Square array of numbers related to the incomplete gamma function,
 *          read by antidiagonals.
 * https://oeis.org/A080955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = value of the square array T(k,m) = m! * * Sum_{j=0..m}
 *            k^j / j! read by antidiagonals reversed along each * diagonal.
 *            The mapping from n to the position (k,m) is: * find the smallest
 *            m with (m+1)(m+2)/2 > n, * previous = m·(m+1)/2, * offset = n -
 *            previous (0 ≤ offset ≤ m) * k = m - offset...
 * Range:     u8 n <= 25, u16 n <= 52, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
seq_t a(idx_t n)
{
    /* Find the diagonal number m. */
    idx_t m = 0;
    for (;;) {
        unsigned __int128 t = (unsigned __int128)(m + 1) * (m + 2) / 2;
        if (t > (unsigned __int128)n)
            break;
        ++m;
    }

    idx_t prev   = m * (m + 1) / 2;   /* terms before this diagonal           */
    idx_t offset = n - prev;          /* position inside the diagonal          */
    idx_t k      = m - offset;        /* row index                            */
    idx_t col    = offset;            /* column index in T(k, col)             */

    /* Compute T(k, col) = col! * Σ_{j=0..col} k^j / j! */
    seq_t term = 1;          /* will grow to col!                     */
    for (idx_t i = 2; i <= col; ++i)
        term *= (seq_t)i;    /* term = col!                           */

    seq_t sum = 0;
    for (idx_t j = 0; j <= col; ++j) {
        sum += term;                    /* add current term                     */
        if (j < col) {                  /* prepare next term: term * k / (j+1) */
            seq_t div = term / (seq_t)(j + 1);
            unsigned __int128 tmp = (unsigned __int128)div * (unsigned __int128)k;
            term = (seq_t)tmp;          /* modulo seq_t width                  */
        }
    }

    return sum;
}
