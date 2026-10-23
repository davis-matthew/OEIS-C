/*
 * A118799  Determinants of 4 X 4 matrices of 16 consecutive primes.
 * https://oeis.org/A118799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = determinant of the 4×4 matrix whose entries are the * 16
 *            consecutive primes beginning with the n-th prime. * i.e. a(n) =
 *            det( [p_{4(i-1)+j}] ), 1 ≤ i,j ≤ 4, where p_k is the k‑th prime
 * Range:     i8 none, i16 n <= 33, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include "../utils/build_primes_upto.h"

/* Global cache of primes (1‑based index).  We cache at least the
 * first 'max_needed' primes.  'primes' points to an array of
 * uint64_t values containing the primes.
 */
static uint64_t *primes = NULL;
static size_t   primes_cnt = 0;
static idx_t   max_needed_primes = 0;

/* Rebuild the prime table so that at least 'need' primes are
 * available.  'need' is the number of primes required for
 * the largest n encountered so far.
 */
static void ensure_primes_at_least(idx_t need)
{
    if (need <= max_needed_primes)
        return;                 /* already enough */

    /* Estimate an upper bound for the 'need'-th prime using
     * the formula p_n < n (log n + log log n) for n >= 6.
     * (If need < 6, take a small fixed bound.)
     */
    idx_t limit;
    if (need < 6)
        limit = 15;
    else
    {
        double nn   = (double)need;
        double bound = nn * (log(nn) + log(log(nn))) + 10.0;
        limit = (idx_t)(bound + 10.0);
    }
    if (limit < 15)
        limit = 15;

    /* Build primes up to 'limit' */
    free(primes);
    build_primes_upto(limit, &primes, &primes_cnt);

    /* In rare cases the bound may be underestimated; rebuild
     * with a larger limit until we have enough primes.
     */
    while (primes_cnt < (size_t)need)
    {
        limit *= 2;
        free(primes);
        build_primes_upto(limit, &primes, &primes_cnt);
    }

    max_needed_primes = need;
}

/* Multiply two seq_t values using __int128 to avoid overflow
 * before the final division.
 */
static inline __int128 mul128(seq_t a, seq_t b)
{
    return (__int128)a * (__int128)b;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* We need 16 consecutive primes starting at index n. */
    idx_t need = n + 15;
    ensure_primes_at_least(need);

    /* Load the 16 primes into a 4×4 matrix. */
    seq_t A[4][4];
    idx_t base = n - 1;               /* zero‑based index of first prime */
    for (idx_t i = 0; i < 4; ++i)
        for (idx_t j = 0; j < 4; ++j)
            A[i][j] = (seq_t)primes[base + i * 4 + j];

    /* Use the Bareiss (fraction‑free) algorithm with row pivoting
     * to compute the determinant exactly.
     */
    seq_t B[4][4];
    for (idx_t i = 0; i < 4; ++i)
        for (idx_t j = 0; j < 4; ++j)
            B[i][j] = A[i][j];

    int sign = 1;                     /* track the sign changes due to row swaps */
    seq_t prev_diag = 1;              /* B[k-1][k-1] for k=0 is 1 */

    for (idx_t k = 0; k < 3; ++k)
    {
        /* Pivot check and possible row swap to avoid 0 pivot */
        if (B[k][k] == 0)
        {
            idx_t r = k + 1;
            while (r < 4 && B[r][k] == 0)
                ++r;
            if (r == 4)            /* column is zero → det = 0 */
                return 0;
            /* swap rows k and r */
            for (idx_t j = 0; j < 4; ++j)
            {
                seq_t tmp = B[k][j];
                B[k][j] = B[r][j];
                B[r][j] = tmp;
            }
            sign = -sign;
        }

        seq_t pivot = B[k][k];
        for (idx_t i = k + 1; i < 4; ++i)
        {
            for (idx_t j = k + 1; j < 4; ++j)
            {
                /* B[i][j] = (pivot * B[i][j] - B[i][k] * B[k][j]) / prev_diag */
                __int128 num = mul128(pivot, B[i][j]) - mul128(B[i][k], B[k][j]);
                B[i][j] = (seq_t)(num / prev_diag);
            }
        }
        prev_diag = pivot;
    }

    seq_t det = B[3][3];
    if (sign < 0)
        det = -det;
    return det;
}
