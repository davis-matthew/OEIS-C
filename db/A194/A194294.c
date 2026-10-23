/*
 * A194294  Triangular array: g(n,k)=number of fractional parts (i*r) in
 *          interval [(k-1)/n, k/n], for 1<=i<=n, 1<=k<=2n, r=(1+sqrt(5))/2,
 *          the golden ratio.
 * https://oeis.org/A194294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   g(n,k) = number of i in [1,2n] such that the fractional part of
 *            i·φ * lies in the interval [(k-1)/n, k/n), where φ = (1+√5)/2. *
 *            * The sequence a(n) is the flattened triangular array * g(1,1),
 *            g(2,1),g(2,2), g(3,1),g(3,2),g(3,3), … * The row index r is
 *            determined by a(n) with a(1) = 2. * * Exact...
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t PHI_NUM = 196418ULL;   /* numerator of a high‑precision φ approx 1.618033988725 */
static const uint64_t PHI_DEN = 121393ULL;   /* denominator of that approximation */

/* Count how many i in [1,2r] have fractional part of i*φ in interval k/n */
static inline seq_t count_interval(idx_t r, idx_t k)
{
    seq_t cnt = 0;
    for (idx_t i = 1; i <= 2 * r; ++i)
    {
        /* compute fractional part of i*φ as (i*PHI_NUM mod PHI_DEN)/PHI_DEN */
        uint64_t num = ((uint64_t)i * PHI_NUM) % PHI_DEN;
        /* Determine which interval this fractional part falls into */
        uint64_t idx = (num * r) / PHI_DEN;          /* floor(num * r / PHI_DEN) */
        if ((idx + 1) == k)
            ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)            /* required externally linked function */
{
    /* find the row r such that r*(r+1)/2 >= n > r*(r-1)/2 */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;
    idx_t prev = (r * (r - 1)) / 2;   /* number of terms before this row */
    idx_t k    = n - prev;            /* column in this row (1‑based) */

    return count_interval(r, k);
}
