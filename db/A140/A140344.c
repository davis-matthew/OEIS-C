/*
 * A140344  Catalan triangle A009766 prepended by n zeros in its n-th row.
 * https://oeis.org/A140344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   Let r = floor(sqrt(n)). Let offset = n - r^2. If offset < r,
 *            then a(n) = 0. Otherwise k = offset - r and a(n) = T(r,k) with
 *            T(r,k) = binom(r+k,k) * (r - k + 1) / (r + 1). Here binom(m,k)
 *            is the binomial coefficient C(m,k)
 * Range:     u8 n <= 23, u16 n <= 79, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

static inline idx_t floor_sqrt_u64(idx_t n)
{
    /* approximate with long double and correct */
    long double d = sqrt((long double)n);
    idx_t r = (idx_t)d;
    while (((__int128)(r + 1)) * (r + 1) <= n) ++r;
    while ((__int128)r * r > n) --r;
    return r;
}

static inline seq_t binom_unsigned(idx_t n, idx_t k)
{
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t r = floor_sqrt_u64(n);            /* r = floor(sqrt(n))           */
    idx_t offset = n - r * r;              /* offset within current row    */
    if (offset < r) return 0;              /* leading zeros                */
    idx_t k = offset - r;                  /* position within Catalan row  */
    seq_t bin = binom_unsigned(r + k, k);   /* binomial part                */
    seq_t val = bin * (r - k + 1);          /* multiply numerator fraction  */
    val = val / (r + 1);                    /* divide denominator           */
    return val;
}
