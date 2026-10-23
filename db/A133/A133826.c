/*
 * A133826  Triangle whose rows are sequences of increasing and decreasing
 *          tetrahedral numbers: 1; 1,4,1; 1,4,10,4,1; ... .
 * https://oeis.org/A133826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = tetrahedral( min(p+1, 2*r-1-p) ), where r = ⌊√n⌋ + 1 and
 *            p = n - (r-1)²
 * Range:     u8 n <= 109, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

static inline idx_t floor_isqrt(idx_t n)
{
    if (n < 2)
        return n;
    long double d = sqrt((long double)n);
    idx_t x = (idx_t)d;
    /* Correct possible off‑by‑one from floating point. */
    while ((x + 1) <= n / (x + 1))
        ++x;
    while (x > n / x)
        --x;
    return x;
}

seq_t a(idx_t n)
{
    /* Determine the row r (1‑based). */
    idx_t r = floor_isqrt(n) + 1;
    /* Starting index of this row. */
    idx_t offset = (idx_t)(((seq_t)(r - 1) * (seq_t)(r - 1)));
    /* Position within the row. */
    idx_t p = n - offset;

    /* Column index k (1‑based). */
    idx_t k = (p < r) ? (p + 1) : (2 * r - 1 - p);

    /* Compute tetrahedral number T(k) = k(k+1)(k+2)/6 */
    seq_t a0 = (seq_t)k;
    seq_t a1 = a0 + 1;
    seq_t a2 = a0 + 2;

    /* Divide by 2 */
    if ((a0 & 1) == 0)
        a0 >>= 1;
    else if ((a1 & 1) == 0)
        a1 >>= 1;
    else
        a2 >>= 1;

    /* Divide by 3 */
    if (a0 % 3 == 0)
        a0 /= 3;
    else if (a1 % 3 == 0)
        a1 /= 3;
    else
        a2 /= 3;

    seq_t val = a0;
    val *= a1;
    val *= a2;
    return val;
}
