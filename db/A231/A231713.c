/*
 * A231713  Square array A(i,j) = the sum of absolute values of digit
 *          differences in the matching positions of the factorial base
 *          representations of i and j, for i >= 0, j >= 0, read by
 *          antidiagonals.
 * https://oeis.org/A231713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) is obtained by reading the square array A(i,j) = sum |digit
 *            differences| of the factorial base representations of i and j,
 *            in antidiagonal order. This function implements that computation
 *            efficiently
 * Range:     u8 n <= 7380, u16 n <= 7380, u32 n <= 7380, u64 n <= 7380,
 *            u128 n <= 7380, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* factorial values up to 20! (fits in unsigned long long) */
static const uint64_t fact[21] = {
    1ULL, 1ULL, 2ULL, 6ULL, 24ULL, 120ULL, 720ULL, 5040ULL,
    40320ULL, 362880ULL, 3628800ULL, 39916800ULL, 479001600ULL,
    6227020800ULL, 87178291200ULL, 1307674368000ULL,
    20922789888000ULL, 355687428096000ULL, 6402373705728000ULL,
    121645100408832000ULL, 2432902008176640000ULL
};

/* integer square root for __int128 values */
static uint64_t isqrt128(__int128 x)
{
    uint64_t lo = 0;
    uint64_t hi = ((uint64_t)1 << 36);          /* > sqrt(8*2^64) */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if ((__int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* factorial‑base digits of n, least significant (1!) first.
   `digits` must point to storage for at least 20 bytes.
   Returns number of significant digits (0 for n==0). */
static unsigned factorial_digits(uint64_t n, unsigned char *digits)
{
    if (n == 0) {
        return 0;
    }
    uint64_t rem = n;
    unsigned maxp = 0;
    while (maxp + 1 < 21 && fact[maxp + 1] <= rem) ++maxp;

    unsigned len = maxp;
    for (int k = maxp; k >= 1; --k) {
        unsigned char d = rem / fact[k];
        digits[k - 1] = d;
        rem -= (uint64_t)d * fact[k];
    }
    return len;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* find diagonal k such that k(k+1)/2 <= n < (k+1)(k+2)/2 */
    uint64_t k = (isqrt128((__int128)8 * n + 1) - 1) >> 1;

    __int128 t = (__int128)k * (k + 1) / 2;
    uint64_t d = n - (uint64_t)t;

    uint64_t i = d;
    uint64_t j = k - d;

    /* factorial digits for i and j */
    unsigned char dig_i[20];
    unsigned char dig_j[20];
    unsigned len_i = factorial_digits(i, dig_i);
    unsigned len_j = factorial_digits(j, dig_j);

    unsigned maxlen = len_i > len_j ? len_i : len_j;
    seq_t sum = 0;
    for (unsigned pos = 0; pos < maxlen; ++pos) {
        unsigned char di = pos < len_i ? dig_i[pos] : 0;
        unsigned char dj = pos < len_j ? dig_j[pos] : 0;
        sum += di > dj ? di - dj : dj - di;
    }
    return (seq_t)sum;
}
