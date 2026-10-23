/*
 * A038312  Triangle whose (i,j)-th entry is binomial(i,j)*10^(i-j)*10^j.
 * https://oeis.org/A038312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(r, j) * 10^r where r = floor((sqrt(8*n+1)-1)/2)
 *            = maximum i with i*(i+1)/2 <= n and j = n - i*(i+1)/2 is the
 *            column within that row
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Find row r such that triangular(r) <= n < triangular(r+1) */
    idx_t low = 0, high = n;
    while (low <= high)
    {
        idx_t mid = low + (high - low) / 2;
        idx_t tri = mid * (mid + 1) / 2;    /* triangular number */
        if (tri <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    idx_t r = high;

    /* Column j in that row */
    idx_t offset = r * (r + 1) / 2;
    idx_t j = n - offset;

    /* Compute the entry: binomial(r, j) * 10^r */
    seq_t bin = binomial(r, j);
    seq_t pow = pow10(r);

    return bin * pow;   /* works for primitive types and overloaded for big ints */
}
