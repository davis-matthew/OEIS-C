/*
 * A143786  Number of arithmetic progressions from m to n; a rectangular
 *          array, m>=0, n>=0, by antidiagonals.
 * https://oeis.org/A143786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of divisors of |p - q| where (p,q) is the
 *            coordinate of the n-th element when the array T(p,q)=d(|p-q|)
 *            (the divisor function) is read by antidiagonals. The coordinate
 *            (p,q) is determined by the unique integers r>=1 and 0<=offset<r
 *            such that n = r(r-1)/2 + offset + 1. Then p = offse...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t divisor_count_uint64(uint64_t m)
{
    if (m == 0) return 1;                 /* convention for d(0) */
    uint64_t result = 1;
    uint64_t count = 1;
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            count = 1;
            while (m % p == 0)
            {
                m /= p;
                ++count;
            }
            result *= count;
        }
    }
    if (m > 1) /* remaining prime factor */
        result *= 2;
    return result;
}

seq_t a(idx_t n)
{
    /* Find diagonal r such that r(r-1)/2 < n <= r(r+1)/2 */
    idx_t r = 0;
    idx_t cum = 0;
    while (cum < n)
    {
        ++r;
        cum += r;
    }
    /* cum == r(r+1)/2, previous cumulative sum is t = (r-1)*r/2 */
    unsigned __int128 prev128 = (unsigned __int128)(r - 1) * r / 2;
    idx_t prev = (idx_t)prev128;          /* previous cumulative indices */

    idx_t offset = n - prev - 1;          /* 0 <= offset < r */

    idx_t p = offset;
    idx_t q = (r - 1) - offset;

    idx_t diff = (q >= p) ? (q - p) : (p - q);

    uint64_t d = divisor_count_uint64((uint64_t)diff);
    return (seq_t)d;
}
