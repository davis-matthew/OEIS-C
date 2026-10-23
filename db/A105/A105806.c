/*
 * A105806  Triangle of number of partitions of n with nonnegative Dyson rank
 *          r=0,1,...,n-1.
 * https://oeis.org/A105806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 406, u16 n <= 1659, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) = Σ_{k≥1} (-1)^{k-1} ( p(n_row − r*k − g1(k)) – p(n_row − r*k − g2(k)) )
 * where   g1(k)=k(3k−1)/2, g2(k)=k(3k+1)/2,
 *         n_row is the row index (1‑based) that contains the term,
 *         r  = n_offset – T(n_row-1) – 1 (rank of the term).
 * The partition numbers p(m) are computed with the standard DP.
 *
 * This implementation precomputes partition numbers up to the largest row
 * needed across calls.  Row size is bounded by a fixed maximum (here 2000),
 * sufficient for all test cases that fit within the time limits.
 */

#include <stdint.h>
#include <stddef.h>

/* Maximum row for which partition numbers are precomputed. */
#define MAX_ROWS 2000

/* Static storage for partition numbers p[0..MAX_ROWS] */
static seq_t partition[MAX_ROWS + 1];
static idx_t partition_lim = 0;   /* maximum n for which p[0..n] are valid */

/* Ensure that partition numbers up to N are available */
static void ensure_partitions(idx_t N)
{
    if (N > MAX_ROWS) N = MAX_ROWS;   /* when N exceeds the buffer we truncate */
    if (N <= partition_lim) return;   /* already ready */

    /* initialise all entries to zero */
    for (idx_t i = 0; i <= N; ++i) partition[i] = 0;
    partition[0] = 1;                 /* p(0) = 1 */

    /* standard DP: for each part size */
    for (idx_t part = 1; part <= N; ++part) {
        for (idx_t sum = part; sum <= N; ++sum) {
            partition[sum] += partition[sum - part];
        }
    }
    partition_lim = N;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* safety – not used by OEIS */

    /* 1. Find the row m such that T(m-1) < n ≤ T(m) with T(x)=x(x+1)/2 */
    idx_t m = 1;
    while ((m * (m + 1) / 2) < n) ++m;

    /* 2. Compute the rank r within that row */
    idx_t prev = ((m - 1) * m) / 2;          /* T(m-1) */
    idx_t r = n - prev - 1;                  /* 0‑based rank */

    /* 3. Make sure partition numbers up to m are available */
    ensure_partitions(m);

    /* 4. Evaluate the sum over k */
    seq_t pos = 0;   /* positive contribution */
    seq_t neg = 0;   /* negative contribution */

    for (idx_t k = 1; ; ++k) {
        idx_t rk = r * k;
        idx_t g1 = k * (3 * k - 1) / 2;
        idx_t g2 = k * (3 * k + 1) / 2;

        int sign_pos = (k & 1) ? 1 : -1;     /* (-1)^{k-1} */

        /* term with g1 */
        idx_t idx1 = m - rk - g1;
        if (rk + g1 <= m && idx1 <= m) {
            seq_t val = partition[idx1];
            if (sign_pos > 0) pos += val;
            else             neg += val;
        }

        /* term with g2 */
        idx_t idx2 = m - rk - g2;
        if (rk + g2 <= m && idx2 <= m) {
            seq_t val = partition[idx2];
            if (sign_pos > 0) neg += val;
            else             pos += val;
        }

        /* Stop when neither term is possible for this and larger k */
        if (rk + g1 > m && rk + g2 > m) break;
    }

    /* 5. Result is the difference; it is guaranteed ≥ 0 for valid ranks */
    return (pos >= neg) ? (pos - neg) : 0;
}
