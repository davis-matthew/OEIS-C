/*
 * A194305  Triangular array: g(n,k) = number of fractional parts (i*Pi) in
 *          interval [(k-1)/n, k/n], for 1 <= i <= n, 1 <= k <= n.
 * https://oeis.org/A194305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened triangular array * produced by g(n,k) = #{
 *            1 <= i <= n : frac(i*Pi) in [(k-1)/n , k/n) }. * The sequence
 *            a(n) is read row‑by‑row: for each n >= 1, * output g(n,1),
 *            g(n,2), …, g(n,n)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static const long double PI = 3.141592653589793238462643383279502884L;

/* Find the row r such that r*(r+1)/2 >= n */
static inline idx_t row_of(idx_t n)
{
    /* r grows roughly as sqrt(2*n). A small loop is fine. */
    idx_t r = 1;
    while ((idx_t)r * (r + 1) / 2 < n)
        ++r;
    return r;
}

/* Count how many i in [1..r] have their fractional part in the k-th interval */
static inline seq_t count_in_interval(idx_t r, idx_t k)
{
    idx_t count = 0;
    for (idx_t i = 1; i <= r; ++i)
    {
        long double frac = fmodl(i * PI, 1.0L);
        if (frac >= 1.0L)    /* protect against rounding to 1.0 */
            frac -= 1.0L;
        idx_t bin = (idx_t)(frac * (long double)r); /* 0‑based bin index */
        if (bin + 1 == k)
            count++;
    }
    return (seq_t)count;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for index 0; return 0 */

    idx_t r = row_of(n);
    idx_t prev_total = (idx_t)r * (r - 1) / 2;
    idx_t k = n - prev_total;        /* column number in this row (1‑based) */

    return count_in_interval(r, k);
}
