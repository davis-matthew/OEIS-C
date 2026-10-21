/*
 * A219731  Irregular triangle M_2(n,k) read by rows: number of maximum
 *          k-matchings in rooted plane trees of size n, 1<=k<=n/2, 2<=n.
 * https://oeis.org/A219731
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A219731:  Irregular triangle M_2(n,k) read by rows.
    Formula for M_2(n,k):
        M(n,k) = sum_{l=max(n-2k,1)}^{n-k} binomial(l, n-2k)
                                       * binomial(2n-3k-l-1, n-k-l)
                                       * binomial(3k+l-2, l-1) / l
    a(n) is the n‑th element of the triangle read row‑wise, with offset 2
    (so a(2) is the first term).  The function below maps the ordinal
    index to the appropriate (n,k) and evaluates the sum using the
    provided binomial routine.  All arithmetic is done in seq_t. */
#include <stdint.h>
#include "../utils/binomial.h"

static seq_t compute_M2(idx_t n, idx_t k)
{
    /* n >= 2, 1 <= k <= n/2 */
    seq_t result = 0;
    idx_t l_start = (n > 2 * k) ? (n - 2 * k) : 1;
    idx_t l_end   = n - k;
    if (l_start > l_end) return result;  /* safety, should not happen */

    for (idx_t l = l_start; l <= l_end; ++l)
    {
        seq_t term = binomial(l, n - 2 * k);

        /* middle binomial: binomial(2n-3k-l-1, n-k-l) */
        long long up   = (long long)2 * n - 3LL * k - (long long)l - 1;
        long long low  = (long long)n - (long long)k - (long long)l;
        seq_t term2;
        if (up < 0)
        {
            /* binomial(negative, 0) is 1; otherwise 0 */
            term2 = (low == 0) ? (seq_t)1 : (seq_t)0;
        }
        else
        {
            term2 = binomial((idx_t)up, (idx_t)low);
        }
        if (term2 == 0) continue;

        /* last binomial: binomial(3k+l-2, l-1) */
        seq_t term3 = binomial(3 * k + l - 2, l - 1);

        term *= term2;
        term *= term3;
        term /= (seq_t)l;
        result += term;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* offset 2: a(2) is the first element of the triangle */
    if (n < 2) return 0;                 /* undefined below offset */

    idx_t target = n - 1;                 /* 1‑based position in flattened list */
    idx_t r = 2, cum = 0;
    while (1)
    {
        cum += r / 2;                     /* number of entries in row r */
        if (cum >= target) break;
        ++r;
    }

    idx_t prev_cum = cum - r / 2;          /* cumulative up to row r‑1 */
    idx_t offset = target - prev_cum - 1;  /* 0‑based position within row r */
    idx_t k = offset + 1;                  /* k is 1‑based within the row */

    return compute_M2(r, k);
}
