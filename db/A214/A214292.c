/*
 * A214292  Triangle read by rows: T(n,k) = T(n-1,k-1) + T(n-1,k), 0 < k < n
 *          with T(n,0) = n and T(n,n) = -n.
 * https://oeis.org/A214292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the row‑major reading of the triangle T(n,k) defined by
 *            T(n,k)=T(n-1,k-1)+T(n-1,k) (0<k<n) T(n,0)=n , T(n,n)=-n
 *            Equivalently T(n,k)=binom(n+1,k)*(n-2k)/(k+1)
 * Range:     i8 n <= 57, i16 n <= 195, i32 n <= 678, i64 n <= 2441,
 *            i128 n <= 9101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Euclidean gcd for unsigned 64‑bit integers. */
static inline uint64_t _gcd_u64(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Find r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
static inline idx_t _row_of(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* locate (row,column) of the n‑th term */
    idx_t r = _row_of(n);                           /* row index */
    idx_t start = (idx_t)(((__int128)r * (r + 1)) / 2);
    idx_t k = n - start;                            /* column in that row */

    /* boundary cases  */
    if (k == 0)               return (seq_t)r;
    if (k == r)               return -(seq_t)r;

    seq_t factor = (seq_t)r - (seq_t)(2 * k);       /* n-2k */
    if (factor == (seq_t)0)    return (seq_t)0;

    int sign = (factor > (seq_t)0) ? 1 : -1;
    uint64_t abs_factor = (factor > (seq_t)0) ? (uint64_t)factor
                                              : (uint64_t)(-factor);

    uint64_t N = (uint64_t)r + 1;                    /* n+1 */
    uint64_t K = (uint64_t)k;                        /* k */

    /* For n up to about 10,000, k ≤ 140, so fixed arrays are safe. */
    uint64_t num[250];
    uint64_t den[250];
    uint64_t len = K + 1;                            /* K numerators + factor */

    /* Numerators of binom(N,K):  N-K+1 ... N  */
    for (uint64_t i = 0; i < K; ++i)
        num[i] = N - K + i + 1;

    num[K] = abs_factor;                            /* extra numerator */

    /* Denominators: 1 ... K  and an extra (K+1) */
    for (uint64_t i = 0; i < K; ++i)
        den[i] = i + 1;
    den[K] = K + 1;

    /* Cancel common factors between numerators and denominators */
    for (uint64_t i = 0; i < len; ++i) {
        for (uint64_t j = 0; j < len; ++j) {
            if (den[j] == 1)
                continue;
            uint64_t g = _gcd_u64(num[i], den[j]);
            if (g > 1) {
                num[i] /= g;
                den[j] /= g;
                if (num[i] == 1)
                    break;
            }
        }
    }

    /* Multiply remaining numerators; small enough for 128‑bit */
    __int128 prod = 1;
    for (uint64_t i = 0; i < len; ++i)
        prod *= (__int128)num[i];

    if (sign < 0)
        prod = -prod;

    return (seq_t)prod;
}
