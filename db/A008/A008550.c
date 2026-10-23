/*
 * A008550  Table T(n,k), n>=0 and k>=0, read by antidiagonals: the k-th
 *          column given by the k-th Narayana polynomial.
 * https://oeis.org/A008550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(row, col) where T(n,k) = sum_{j=0}^{k-1} (binom(k,
 *            j+1)*binom(k, j)/k) * n^j row = offset of n inside its
 *            anti‑diagonal, col = diagonal index minus offset. The triangular
 *            array T has T(n,0)=1. The values are the Narayana polynomial
 *            evaluated at n
 * Range:     u8 n <= 36, u16 n <= 67, u32 n <= 155, u64 n <= 381,
 *            u128 n <= 996, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/binomial.h"  /* binomial(idx_t, idx_t) -> seq_t */

/* helper: floor of rational k*(k+1)/2 for idx_t k */
static inline unsigned __int128 cum(idx_t k)
{
    return (unsigned __int128)k * (k + 1) / 2;
}

/* compute the anti‑diagonal index s for a given position n */
static idx_t diag_index(idx_t n)
{
    /* 8*n+1 fits into 128 bits for all 64‑bit n */
    unsigned __int128 m = (unsigned __int128)n * 8 + 1;
    long double d = sqrt((long double)m);
    idx_t s = (idx_t)((d - 1.0L) / 2.0L);          /* initial guess */

    /* adjust to satisfy cum(s) <= n < cum(s+1) */
    while (cum(s + 1) <= n) s++;
    while (cum(s) > n) s--;
    return s;
}

/* compute T(row, col) as described above */
static seq_t T_value(idx_t row, idx_t col)
{
    if (col == 0) return (seq_t)1;

    seq_t result = 0;
    seq_t power   = 1;            /* row^j */
    seq_t row_val = (seq_t)row;   /* cast once for speed */

    for (idx_t j = 0; j < col; ++j)
    {
        seq_t a = binomial(col, j + 1);   /* C(col, j+1) */
        seq_t b = binomial(col, j);       /* C(col, j)   */
        seq_t coeff = (a * b) / (seq_t)col; /* Narayana number N(col, j+1) */
        result += coeff * power;
        power *= row_val;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t s     = diag_index(n);
    idx_t prev  = (idx_t)cum(s);            /* number of terms before diagonal s */
    idx_t offset = n - prev;                /* 0‑based position inside diagonal s */
    idx_t row   = offset;                   /* row index for T */
    idx_t col   = s - offset;               /* column index for T */
    return T_value(row, col);
}
