/*
 * A046899  Triangle in which n-th row is {binomial(n+k,k), k=0..n}, n >= 0.
 * https://oeis.org/A046899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 63, u32 n <= 187, u64 n <= 628,
 *            u128 n <= 2276, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = binomial(r + k, k) where
   r = floor((sqrt(8*n+1)-1)/2)  (the row number)
   k = n - r*(r+1)/2             (the column number)
   The triangle entries are T(r,k)=binomial(r+k,k). */

#include <stdint.h>
#include "../utils/gcd.h"

/* triangular number: T(r)=r*(r+1)/2  (fits in idx_t for n<=10000) */
static inline idx_t triangular(idx_t r)
{
    return r * (r + 1) / 2;
}

/* multiplicative binomial implementation using reduction to avoid overflow.
   Works for any seq_t type supported by the utilities. */
static inline seq_t binomial_choose(idx_t n, idx_t k)
{
    if (k > n - k)   /* use symmetry */
        k = n - k;
    if (k == 0)
        return (seq_t)1;

    /* 200 is safe for n up to 10^4 because k <= sqrt(2n) < 140. */
    seq_t numerators[200];
    for (idx_t i = 0; i < k; ++i)
        numerators[i] = (seq_t)(n - k + 1 + i);

    /* Cancel denominators 2..k with numerators via GCD. */
    for (idx_t d = 2; d <= k; ++d) {
        idx_t t = d;
        for (idx_t i = 0; i < k && t > 1; ++i) {
            seq_t g = gcd((seq_t)t, numerators[i]);
            if (g > (seq_t)1) {
                numerators[i] /= g;
                t /= (idx_t)g;
            }
        }
    }

    /* Multiply the remaining numerators.  No intermediate overflow occurs
       because the partial product never exceeds the final value. */
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < k; ++i)
        res = res * numerators[i];
    return res;
}

seq_t a(idx_t n)
{
    /* Find row r such that triangular(r) <= n < triangular(r+1). */
    idx_t low = 0, high = 1;
    while (triangular(high) <= n)
        high <<= 1; /* exponential grow until high too large */

    high--; /* now high is the last r with triangular(r) <= n */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (triangular(mid) <= n) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    idx_t r = high;
    idx_t k = n - triangular(r);
    return binomial_choose(r + k, k);
}
