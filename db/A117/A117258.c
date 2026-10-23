/*
 * A117258  Triangle T, read by rows, where matrix power T^2 has 2*4^n in the
 *          secondary diagonal: [T^2](n+1,n) = 2*4^n, with all 1's in the main
 *          diagonal and zeros elsewhere.
 * https://oeis.org/A117258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(n,k) = (-1)^(n-k) * 4^(n*(n-1)/2 - k*(k-1)/2) / * (n-k)! *
 *            prod_{j=0..n-k-1}(2*j-1) (n>k>=0), T(n,n)=1 * a(idx) is the
 *            triangle read by rows: a(idx) = T(r,k) where * row r is the
 *            smallest integer with idx < (r+1)(r+2)/2 and k = idx - r(r+1)/2
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 20, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine row r and column k from the global index n */
    idx_t r = 0;
    idx_t cum = 0;
    while (1) {
        idx_t row_len = r + 1;
        if (cum + row_len > n) break;
        cum += row_len;
        r++;
    }
    idx_t k = n - cum;

    /* Diagonal entries are always 1. */
    if (k == r) return 1;

    /* Distance from diagonal */
    idx_t d = r - k;

    /* Sign = (-1)^d */
    seq_t sign = ((d & 1) ? -1 : 1);

    /* Compute exponent E = r*(r-1)/2 - k*(k-1)/2 (as seq_t) */
    seq_t E = ((seq_t)r * (seq_t)(r - 1) / 2)
            - ((seq_t)k * (seq_t)(k - 1) / 2);

    /* Compute 4^E using exponentiation by squaring */
    seq_t pow4 = 1;
    seq_t base  = 4;
    idx_t exp   = (idx_t)E;          /* E fits in idx_t for all needed n */
    while (exp) {
        if (exp & 1) pow4 *= base;
        base *= base;
        exp >>= 1;
    }

    /* Compute factorial d! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= d; ++i)
        fact *= (seq_t)i;

    /* Compute product prod_{j=0..d-1}(2*j-1) */
    seq_t prod = 1;
    for (idx_t j = 0; j < d; ++j)
        prod *= ((seq_t)(2 * j) - 1);

    /* Assemble the value */
    seq_t val = sign * pow4;
    val = val * prod;
    val = val / fact;   /* division is exact by formula */

    return val;
}
