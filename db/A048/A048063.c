/*
 * A048063  Number of nonempty subsets of {1,2,...,n} in which exactly 1/4 of
 *          the elements are <= (n-4)/2.
 * https://oeis.org/A048063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{K} binom(l, k) * binom(r, 3k) * where l = max(0,
 *            floor((n-4)/2)), r = n - l, and * K = min(l, floor(r/3)). The
 *            sum counts non‑empty subsets of * {1,…,n} containing exactly one
 *            quarter of their elements * from {1,…,l}
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the provided binomial routine. */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute l = floor((n-4)/2) but not negative. */
    idx_t l = 0;
    if (n > 3)
        l = (n - 4) / 2;      /* integer division is floor for positive num */
    idx_t r = n - l + 1;     /* because elements are 1..n inclusive */
    /* Correct r: number of elements > l is n - l */
    r = n - l;

    /* Determine the upper bound for k. */
    idx_t K = l;
    idx_t alt = r / 3;
    if (alt < K) K = alt;
    if (K == 0) return 0;   /* No k possible => no valid subset */

    seq_t sum = 0;
    for (idx_t k = 1; k <= K; ++k) {
        seq_t c1 = binomial(l, k);
        seq_t c2 = binomial(r, 3 * k);
        sum += c1 * c2;
    }
    return sum;
}
