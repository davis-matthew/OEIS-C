/*
 * A155537  Triangle T(n,k,p,q) = (p^n + q^n)*A001263(n, k) with p=2 and q=1,
 *          read by rows.
 * https://oeis.org/A155537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 31, u32 n <= 96, u64 n <= 310,
 *            u128 n <= 1101, big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A155537
 * Formula: a(n) = T(r,k) where r = smallest integer with r*(r+1)/2 ≥ n,
 *          k = n - (r-1)*r/2,
 *          and T(r,k) = (2^r + 1) * binom(r-1,k-1) * binom(r,k) / (r-k+1),
 *          read by rows of the triangle. */
#include "../utils/gcd.h"
#include "../utils/pow2.h"

/* Compute binomial coefficient C(n,k) for n,k ≥ 0 using
 * an incremental multiplicative algorithm. The intermediate
 * products are guaranteed to be integers, so the division
 * is exact. */
static seq_t binom_simple(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
        result = result * (n - k + i) / i;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1; unused */
        return 0;

    /* Find r such that r*(r+1)/2 ≥ n using binary search */
    idx_t low = 1, high = 1;
    while (high * (high + 1) / 2 < n)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;
    idx_t prev_terms = (r - 1) * r / 2;
    idx_t k = n - prev_terms;

    /* Triangle entry */
    seq_t pow2r = pow2(r);            /* 2^r */
    seq_t factor = pow2r + 1;         /* 2^r + 1 */

    seq_t a1 = binom_simple(r - 1, k - 1);
    seq_t a2 = binom_simple(r, k);

    seq_t denom = (seq_t)(r - k + 1);

    /* Reduce numerator and denominator to keep intermediates small */
    seq_t g = gcd(a1, denom);
    a1 /= g;
    denom /= g;

    g = gcd(a2, denom);
    a2 /= g;
    denom /= g; /* should be 1 now */

    seq_t narayana = a1 * a2;
    if (denom != 1)
        narayana /= denom;

    return factor * narayana;
}
