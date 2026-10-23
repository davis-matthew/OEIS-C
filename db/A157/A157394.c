/*
 * A157394  A partition product of Stirling_1 type [parameter k = 4] with
 *          biggest-part statistic (triangle read by rows).
 * https://oeis.org/A157394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 30, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A157394 using a recurrence.
 * Let f_k = product_{j=0}^{k-2} (j-k+6)  (f_k = 1,4,12,24,24 for k=1..5, f_k=0 for k>5).
 * For d=0..5 define B_d(0)=1 and for n≥1
 *       B_d(n) = Σ_{k=1}^{min(n,d)} C(n-1,k-1) * f_k * B_d(n-k)
 *       (C is the binomial coefficient).
 * Then T(n,m) = B_m(n) – B_{m-1}(n) (T(n,m)=0 for m>5).
 * The sequence a(·) is the triangle T read row by row, i.e.
 *   a(index) = T(r,c)  where r is the row with r(r+1)/2≥index
 *   and c = index – r(r-1)/2  (1‑based column).
 */

#include <stdint.h>

/* Adjust the following limit if n can be larger on some platforms. */
#define MAX_ROWS 20000

static const seq_t f[6] = { 0, 1, 4, 12, 24, 24 };

/* Small binomial coefficient:
 * C(n,k) for k=0..4 (here k is small because m≤5).
 */
static seq_t binom_small(idx_t n, idx_t k)
{
    switch (k)
    {
      case 0: return (seq_t)1;
      case 1: return (seq_t)n;
      case 2: return ((seq_t)n * (n - 1)) / 2;
      case 3: return ((seq_t)n * (n - 1) * (n - 2)) / 6;
      case 4: return ((seq_t)n * (n - 1) * (n - 2) * (n - 3)) / 24;
      default: return (seq_t)1;          /* should not occur for k≤4 */
    }
}

/* Store B_d(n) for d=0..5 and n up to MAX_ROWS. */
static seq_t B[6][MAX_ROWS + 1];
static idx_t maxComputed = 0;
static int initialized = 0;

/* Return the value a(n) of A157394 for the given index n (1‑based). */
seq_t a(idx_t n)
{
    if (!initialized)
    {
        /* initialise all B_d(0) = 1, other entries are 0 by static init */
        for (int d = 0; d < 6; ++d)
            B[d][0] = (seq_t)1;
        initialized = 1;
    }

    /* Determine row r such that r(r+1)/2 ≥ n */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* Extend pre‑computation to row r if necessary */
    if (r > maxComputed)
    {
        for (idx_t row = maxComputed + 1; row <= r; ++row)
        {
            for (int d = 1; d <= 5; ++d)         /* d = m (1…5) */
            {
                seq_t sum = (seq_t)0;
                idx_t maxk = (row < (idx_t)d) ? row : (idx_t)d;
                for (idx_t k = 1; k <= maxk; ++k)
                {
                    seq_t binom = binom_small(row - 1, k - 1);
                    seq_t term  = binom * f[k];
                    term        *= B[d][row - k];
                    sum         += term;
                }
                B[d][row] = sum;
            }
            /* B[0][row] remains 0 for row > 0 (already zeroed) */
        }
        maxComputed = r;
    }

    /* Column c = index within the row (1‑based) */
    idx_t prevOffset = (r - 1) * r / 2;
    idx_t c = n - prevOffset;         /* 1 ≤ c ≤ r */

    /* If largest part > 5, contribution is zero */
    if (c > 5) return (seq_t)0;

    /* T(r,c) = B[c][r] – B[c-1][r] */
    return B[c][r] - B[c - 1][r];
}
