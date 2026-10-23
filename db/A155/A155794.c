/*
 * A155794  Triangle read by rows: t(n,m)=(m*(m-n))!
 * https://oeis.org/A155794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = factorial( |m*(r-m)| ) where r = floor((sqrt(8*n+1)-1)/2)
 *            and m = n - r*(r+1)/2. t(r,m) = |m*(m-r)|! is the triangle read
 *            by rows; a(n) is the flattened sequence obtained by reading the
 *            rows from left to right
 * Range:     u8 n <= 16, u16 n <= 23, u32 n <= 38, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_P 5000   /* sufficient for n up to about 10000 */

static seq_t fact_cache[MAX_P + 1];
static idx_t fact_cache_size = 0;
static bool fact_cache_init = false;

static void ensure_factorial_cache(idx_t p)
{
    if (!fact_cache_init) {
        fact_cache[0] = (seq_t)1;
        fact_cache_size = 0;
        fact_cache_init = true;
    }
    if (p <= fact_cache_size) return;
    if (p > MAX_P) {
        /* should never happen for the intended limits */
        return;
    }
    for (idx_t i = fact_cache_size + 1; i <= p; ++i) {
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }
    fact_cache_size = p;
}

seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = (idx_t)((sqrt((double)(8 * n + 1)) - 1.0) / 2.0);
    /* correct possible rounding errors */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    /* column index within the row */
    idx_t m = n - r * (r + 1) / 2;

    /* argument of the factorial */
    idx_t p = m * (r - m);

    ensure_factorial_cache(p);
    return fact_cache[p];
}
