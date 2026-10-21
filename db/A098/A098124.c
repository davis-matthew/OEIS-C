/*
 * A098124  Number of compositions of n in which the largest part is equal to
 *          the number of parts.
 * https://oeis.org/A098124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A098124:  Number of compositions of n in which the largest part
 *  equals the number of parts.  For a given n, a(n) = Σ_{k=1..n}
 *  (C(k,k,n) – C(k,k‑1,n)),
 *  where C(k,M,n) denotes the number of compositions of n into exactly
 *  k positive parts each at most M:
 *          C(k,M,n) = Σ_{j=0}^{⌊(n–k)/M⌋} (-1)^j * binom(k,j) *
 *                     binom(n – j*M – 1, k – 1).
 *  The implementation uses the binom routine from "../utils/binom.h".
 */

#include "../utils/binom.h"
#include <stdint.h>

/* number of compositions of n into k parts each ≤ M (k ≥ 1)  */
static seq_t comp_leq(idx_t k, idx_t M, idx_t n)
{
    if (k == 0)                          /* not used, but keep defensively */
        return (n == 0) ? 1 : 0;

    if (M == 0)                          /* no part can be ≥ 1 → impossible */
        return 0;

    /* Minimal sum with k parts of 1 is k; maximal sum with each part ≤ M is k*M */
    if (n < k || n > (seq_t)k * (seq_t)M)
        return 0;

    idx_t jmax = (n - k) / M;            /* floor((n‑k)/M) */
    seq_t total = 0;

    for (idx_t j = 0; j <= jmax; ++j)
    {
        seq_t term = binom(k, j);

        /* remaining sum after subtracting j parts that exceed M by at least 1 */
        idx_t t = n - j * M - 1;          /* = n – j*M – 1 */
        if (t < k - 1)                    /* binom would be zero otherwise */
            break;

        term *= binom(t, k - 1);

        if (j & 1)
            total -= term;
        else
            total += term;
    }
    return total;
}

/* external function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0)                      /* undefined, but return 0 */
        return 0;

    seq_t result = 0;

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t with_k   = comp_leq(k, k, n);     /* parts ≤ k */
        seq_t with_km1 = (k == 1) ? 0 : comp_leq(k, k - 1, n);   /* parts ≤ k‑1 */

        result += with_k - with_km1;
    }
    return result;
}
