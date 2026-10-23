/*
 * A173424  Triangle read by rows: T(n, k) = (2*n - 2*k)!*(2*k)!/(2^n*(n -
 *          k)!*k!).
 * https://oeis.org/A173424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ((2*(n-k))! * (2k)!) / (2^n * (n-k)! * k!), where for
 *            a(n) read from the triangle we first compute the triangle row r
 *            and column k: • r = the largest integer such that r*(r+1)/2 ≤ n
 *            • k = n - r*(r+1)/2
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 44, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t find_row(idx_t idx)
{
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= idx)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* locate triangle coordinates (r,k) */
    idx_t r = find_row(n);
    idx_t k = n - r * (r + 1) / 2;
    idx_t m = r - k;                    /* m = n - k */

    /* compute product (2m)! / m! */
    seq_t prod1 = 1;
    for (idx_t i = m + 1; i <= 2 * m; ++i)
        prod1 *= (seq_t)i;

    /* compute product (2k)! / k! */
    seq_t prod2 = 1;
    for (idx_t i = k + 1; i <= 2 * k; ++i)
        prod2 *= (seq_t)i;

    /* divide by 2^r */
    seq_t denom = ((seq_t)1 << r);

    /* final term */
    seq_t result = prod1 * prod2 / denom;
    return result;
}
