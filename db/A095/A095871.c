/*
 * A095871  Triangle read by rows: T(n,k)=(n+1)*(3*(n+1)-1)/2-k*(3*k-1)/2.
 * https://oeis.org/A095871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{j=1}^{r} (3*j-2) - sum_{j=1}^{k-1} (3*j-2) where r =
 *            ceil( (√(8*n+1)-1)/2 ) is the row containing the nth term and k
 *            = n - r*(r-1)/2 is its column index (1‑based within the row)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 by spec; for n == 0 we return 0 (offset 1). */
    if (n == 0) return (seq_t)0;

    /* Find the row r such that r*(r+1)/2 >= n.  Use binary search
       with 128‑bit intermediates to avoid overflow. */
    idx_t low  = 0;
    idx_t high = 1;
    while (( (__uint128_t)high * (high + 1) ) / 2 < n)
        high <<= 1;                       /* double until enough */

    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ( ( (__uint128_t)mid * (mid + 1) ) / 2 >= n )
            high = mid;
        else
            low = mid;
    }
    idx_t r = high;  /* row number (1‑based) */

    /* Column index k (1‑based within the row). */
    idx_t prev = ( (__uint128_t)r * (r - 1) ) / 2;
    idx_t k = n - prev;

    /* Compute the term using the closed form. */
    seq_t rr = (seq_t)r;
    seq_t kk = (seq_t)k;
    /* sum_{j=1}^{m} (3*j-2) = m*(3*m-1)/2. */
    seq_t sum_r = rr * ((seq_t)3 * rr - (seq_t)1) / (seq_t)2;
    seq_t mm = kk - (seq_t)1;
    seq_t sum_k_1 = mm * ((seq_t)3 * mm - (seq_t)1) / (seq_t)2;

    return sum_r - sum_k_1;
}
