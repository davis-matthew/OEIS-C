/*
 * A095881  Triangle read by rows: T(n,k) = (n-k+1)^(n^2), n>=1, 1<=k<=n.
 * https://oeis.org/A095881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (r - c + 1)^(r^2) where * r = smallest integer with
 *            r*(r+1)/2 ≥ n, * c = n - r*(r-1)/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 15, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is compatible with C11 and C++17.  It relies only on
 * the arithmetic operators defined for seq_t and uses the integer type
 * idx_t for indexing.  No external libraries are required. */
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n.  Since n <= 10000 (for
     * our performance constraint), a simple linear search is fast enough. */
    idx_t r = 0;
    idx_t cum = 0;
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* Compute column k (1‑based) within the row r. */
    idx_t prev_cum = cum - r;        /* sum of first (r-1) rows */
    idx_t k = n - prev_cum;          /* index within this row (1..r) */

    /* Base of the power: (r - k + 1).  Convert to seq_t. */
    seq_t base = (seq_t)(r - k + 1);

    /* Exponent: r^2.  Compute using idx_t and cast to seq_t. */
    seq_t exp = (seq_t)r * r;

    /* Binary exponentiation: result = 1; while exp > 0. */
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & (seq_t)1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return result;
}
