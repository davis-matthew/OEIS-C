/*
 * A156045  Triangle T(n, k) = 2 + n! - k! - (n-k)!, read by rows.
 * https://oeis.org/A156045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 91, u64 n <= 231,
 *            u128 n <= 630, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A156045 – Triangle read by rows.
   Formula: a(n) = 2 + r! – k! – (r – k)!, where
   r = floor((sqrt(8*n + 1) – 1)/2),
   k = n – r*(r + 1)/2.  */
#include <stdint.h>

static seq_t factorial(idx_t m)
{
    seq_t f = 1;
    for (idx_t i = 1; i <= m; ++i)
        f = f * (seq_t)i;
    return f;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r     = 0;
    idx_t sum   = 0;          /* number of terms before the current row   */
    while (sum + r + 1 <= n) {
        ++r;
        sum += r;
    }

    /* Position k within the row */
    idx_t k = n - sum;

    /* Compute the triangle entry T(r, k) */
    seq_t fact_r   = factorial(r);
    seq_t fact_k   = factorial(k);
    seq_t fact_rk  = factorial(r - k);

    seq_t val = fact_r + 2;
    val -= fact_k;
    val -= fact_rk;
    return val;
}
