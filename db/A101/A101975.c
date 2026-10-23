/*
 * A101975  Triangle read by rows: number of Dyck paths of semilength n with k
 *          peaks after the first return (0 <= k < n).
 * https://oeis.org/A101975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n)-th element of the triangle of Dyck paths. Let r
 *            be the smallest integer with r*(r+1)/2 ≥ n, and let k = n -
 *            (r-1)*r/2 - 1. Then a(n) = T(r,k) where T(r,0) = Catalan(r-1),
 *            T(r,1) = Σ_{i=0}^{r-2} Catalan(i), T(r,k) = Σ_{j=0}^{r-2}
 *            Catalan(j) * binom(r-1-j, k-1) * binom(r-1-j, k) / (...
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   binom_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_len.h"

/* ------------------------------------------------------------ */
/* multiplicative binomial coefficient (indices are idx_t).     */

/* ------------------------------------------------------------ */
/* compute a(idx) – entry of the OEIS sequence.                */
/* ------------------------------------------------------------ */
seq_t a(idx_t idx)
{
    if (idx < (idx_t)1) return (seq_t)0;   /* undefined for 0 */

    /* 1. locate row r: smallest r with r*(r+1)/2 ≥ idx */
    idx_t r = 1;
    while ( (r * (r + 1)) / 2 < idx ) ++r;

    /* 2. locate column k inside that row */
    idx_t prev_terms = (r - 1) * r / 2;      /* terms before row r    */
    idx_t k = idx - prev_terms - (idx_t)1;  /* 0‑based column        */

    /* ---------------------------------------------------------------- */
    /* Compute the required triangle entry T(r,k) using the definition. */
    /* ---------------------------------------------------------------- */

    /* Catalan numbers are built iteratively: C_0=1, C_{i}=C_{i-1}*2*(2*i-1)/(i+1) */
    if (k == (idx_t)0) {
        /* T(r,0) = C_{r-1} */
        seq_t c = (seq_t)1;               /* C_0 */
        for (idx_t i = 1; i <= r - 1; ++i) {
            c *= (seq_t)2 * (seq_t)(2 * i - 1);
            c /= (seq_t)(i + 1);
        }
        return c;
    }

    if (k == (idx_t)1) {
        /* T(r,1) = Σ_{i=0}^{r-2} C_i */
        seq_t sum = (seq_t)0;
        seq_t c = (seq_t)1;               /* C_0 */
        sum += c;
        for (idx_t i = 1; i <= r - 2; ++i) {
            c *= (seq_t)2 * (seq_t)(2 * i - 1);
            c /= (seq_t)(i + 1);
            sum += c;
        }
        return sum;
    }

    /* k >= 2 */
    seq_t sum = (seq_t)0;
    seq_t c = (seq_t)1;                   /* C_0 */
    for (idx_t j = 0; j <= r - 2; ++j) {
        idx_t m = r - 1 - j;              /* m ≥ 1        */
        if (k <= m) {
            seq_t term = c;
            term *= binom_len(m, k - 1);
            term *= binom_len(m, k);
            term /= (seq_t)m;
            sum += term;
        }
        /* update C_{j+1} for the next iteration, if needed */
        if (j < r - 2) {
            idx_t i = j + 1;              /* next index */
            c *= (seq_t)2 * (seq_t)(2 * i - 1);
            c /= (seq_t)(i + 1);
        }
    }
    return sum;
}
