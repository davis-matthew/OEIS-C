/*
 * A105851  Binomial transform triangle, read by rows.
 * https://oeis.org/A105851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2 + c*(r-c)) * 2^(r - c - 1) where r = floor((sqrt(8*n +
 *            1) - 1)/2) and c = n - r*(r+1)/2. This computes the n‑th term of
 *            OEIS A105851 (binomial‑transform triangle read by rows) using
 *            the integer triangular index
 * Range:     u8 n <= 28, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static idx_t row_of_index(idx_t n)
{
    /* initial estimate via floating point */
    unsigned __int128 d = (unsigned __int128)8 * n + 1;
    long double sqrt_d = sqrt((long double)d);
    unsigned long long r = (unsigned long long)((sqrt_d - 1.0L) / 2.0L);

    /* adjust to satisfy inequalities */
    while ((unsigned __int128)r * (r + 1) / 2 > n) r--;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) r++;

    return (idx_t)r;
}

seq_t a(idx_t n)
{
    /* Determine row and column of the triangle entry */
    idx_t r = row_of_index(n);
    idx_t tri = r * (r + 1) / 2;
    idx_t c = n - tri;

    /* exponent of 2 may be negative (only for c == r) */
    int64_t exp = (int64_t)r - (int64_t)c - 1;

    /* numerator = 2 + c*(r-c) */
    seq_t numerator = (seq_t)2 + (seq_t)c * ((seq_t)(r - c));

    /* apply the power of 2 */
    seq_t result;
    if (exp >= 0) {
        result = numerator << exp;
    } else {
        result = numerator >> (-exp);
    }

    return result;
}
