/*
 * A194396  Numbers m such that Sum_{k=1..m} (<1/2 + k*r> - <k*r>) = 0, where
 *          r=sqrt(14) and < > denotes fractional part.
 * https://oeis.org/A194396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m, where m is the smallest integer such that among k=1..m
 *            the numbers floor(k*sqrt(14)) + 1/2 cross the integer boundary
 *            the same number of times from below and from above. In practice:
 *            let diff(k) = +1 if fractional part of k*sqrt(14) < 0.5 else -1.
 *            Then a(n) is the n‑th integer m with Σ_...
 * Range:     u8 n <= 1, u16 n <= 16, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for arbitrary-precision seq_t. */
static inline seq_t isqrt_u128(seq_t n)
{
    seq_t low = 0;
    seq_t high = n;
    while (low < high) {
        seq_t mid = (low + high + 1) / 2;
        if (mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Sequence value a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for 0, return 0 as a safeguard */
    idx_t found = 0;        /* how many terms already found */
    int diff = 0;           /* cumulative diff if f_k<0.5 (+1) else -1   */
    seq_t m = 0;            /* current k that satisfies the condition */

    /* scan k progressively until we have found the nth such m */
    for (seq_t k = 1; found < n; ++k) {
        /* Compute floor(k*sqrt(14)) using integer sqrt of 14*k^2 */
        seq_t kk = k;
        seq_t N14 = kk * kk;
        N14 = N14 * (seq_t)14;
        seq_t L = isqrt_u128(N14);            /* L = floor(k*sqrt(14)) */

        /* test if fractional part < 0.5: 56*k^2 < (2*L+1)^2   */
        seq_t x  = L + L + (seq_t)1;          /* x = 2*L + 1   */
        seq_t N56 = kk * kk;
        N56 = N56 * (seq_t)56;
        if (N56 < x * x)
            ++diff;   /* f_k < 0.5  */
        else
            --diff;   /* f_k >= 0.5 */

        if (diff == 0) {
            ++found;
            m = k;
        }
    }
    return m;
}
