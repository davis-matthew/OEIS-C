/*
 * A093127  Triangle read by rows: differences of Narayana numbers.
 * https://oeis.org/A093127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   T(n,k) = Narayana(n-k+2,k+1) - Narayana(n-k+1,k) * where
 *            Narayana(m,k) = binomial(m,k)*binomial(m,k-1)/m * a(n) is
 *            obtained by reading the triangle by rows
 * Range:     u8 n <= 26, u16 n <= 75, u32 n <= 216, u64 n <= 715,
 *            u128 n <= 2574, big any n
 * Validated: standard, pass, 80 terms through n = 2451 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* number of entries up to and including row r   */
static inline seq_t cum_inc(idx_t r)
{
    idx_t m;
    if ((r & 1) == 0) {                /* r = 2m  */
        m = r >> 1;
        return (seq_t)(m + 1) * (seq_t)(m + 1);   /* (m+1)²    */
    } else {                           /* r = 2m+1  */
        m = (r - 1) >> 1;
        return (seq_t)(m + 1) * (seq_t)(m + 2);   /* (m+1)(m+2) */
    }
}

/* compute Narayana(m,k) safely using recurrence.
 * N(m,1) = 1. For k >= 2:
 *   N(m,k) = N(m,k-1) * (m-k+2)*(m-k+1) / (k*(k-1))
 * All intermediate arithmetic is done in unsigned __int128 to
 * avoid overflow while the final result fits in seq_t. */
static inline seq_t narayana(idx_t m, idx_t k)
{
    if (k == 0 || k > m)
        return 0;
    seq_t N = 1;
    for (idx_t i = 2; i <= k; ++i) {
        unsigned __int128 num = (unsigned __int128)N;
        num *= (m - i + 2);
        num *= (m - i + 1);
        unsigned __int128 den = (unsigned __int128)i * (i - 1);
        N = (seq_t)(num / den);           /* exact integer division */
    }
    return N;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* locate the row r that contains index n */
    idx_t r = 0;
    while (cum_inc(r) <= (seq_t)n)
        ++r;

    /* start index of this row (cumulative up to previous row) */
    seq_t prev = (r == 0) ? 0 : cum_inc(r - 1);

    /* column index k within the row (0‑based) */
    seq_t k_seq = (seq_t)n - prev;
    idx_t k = (idx_t)k_seq;

    /* compute the triangle entry T(r,k) */
    seq_t t1 = narayana(r - k + 2, k + 1);
    seq_t t2 = narayana(r - k + 1, k);
    return t1 - t2;
}
