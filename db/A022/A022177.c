/*
 * A022177  Triangle of Gaussian binomial coefficients [ n,k ] for q = 13.
 * https://oeis.org/A022177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 17, u64 n <= 47, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS sequence A022177: Triangle of Gaussian binomial coefficients
 * [ n,k ] for q = 13, flattened row‑wise.
 *
 * This implementation computes a(n) directly from the definition:
 *   T(n,k) = T(n-1,k-1) + 13^k * T(n-1,k)
 * with T(n,0)=T(n,n)=1.
 * The flattened index n is translated to row r and column k by
 * searching for r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.
 * Then the Gaussian binomial coefficient [r,k] is evaluated
 * multiplicatively:
 *   [r,k] = Π_{i=1..k} (13^{r+1-i} – 1)/(13^i – 1)
 *
 * The algorithm uses only basic arithmetic on seq_t and scales
 * in O(r·k) time with r≈√(2·n); for n≈10000 this is easily
 * within the time limits.  No dynamic caching is required,
 * ensuring correct results for calls in any order.
 */

#include <stdlib.h>   /* malloc, free, realloc */
#include <stdint.h>   /* for size_t, uint64_t */

/* Function prototype supplied by the build */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Cumulative count before row r: r*(r+1)/2 */
    idx_t before = (r * (r + 1)) / 2;
    idx_t c = n - before;   /* column index within the row (0‑based) */

    /* Edge cases: first or last element of the row */
    if (c == 0 || c == r)
        return (seq_t)1;

    /* Precompute powers of 13 up to exponent r+1 */
    idx_t pow_len = r + 2;
    seq_t *pow13 = (seq_t *)malloc(pow_len * sizeof(seq_t));
    if (!pow13)  /* allocation failure – return zero as a safety value */
        return (seq_t)0;
    pow13[0] = (seq_t)1;
    for (idx_t i = 1; i < pow_len; ++i)
        pow13[i] = pow13[i - 1] * (seq_t)13;

    /* Compute the Gaussian binomial coefficient multiplicatively */
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= c; ++i) {
        seq_t numerator   = pow13[r + 1 - i] - (seq_t)1;
        seq_t denominator = pow13[i] - (seq_t)1;
        result = result * numerator / denominator;
    }

    free(pow13);
    return result;
}
