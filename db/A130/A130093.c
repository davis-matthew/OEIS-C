/*
 * A130093  A051731 * a lower triangular matrix with A036987 on the main
 *          diagonal and the rest zeros.
 * https://oeis.org/A130093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if the column index j of the lower‑triangular matrix *
 *            (obtained from flattening i*(i-1)/2 < n <= i*(i+1)/2) is a power
 *            of two * and ((i-1) & (j-1)) == (j-1); otherwise a(n) = 0
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* Undefined for offset 0, return 0 */
        return (seq_t)0;

    /* Find the row i such that i*(i+1)/2 >= n > (i-1)*i/2 */
    idx_t low = 1, high = 1;
    while ( ( (__int128)high * (high + 1) ) / 2 < n )
        high <<= 1;        /* double until we exceed n */

    /* Binary search for the smallest i with triangular(i) >= n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ( ( (__int128)mid * (mid + 1) ) / 2 >= n )
            high = mid;
        else
            low = mid + 1;
    }
    idx_t i = low;

    /* Column index j in row i: j = n - triangular(i-1) */
    idx_t tri_prev = ( (__int128)(i - 1) * i ) / 2;
    idx_t j = n - tri_prev;   /* 1‑based column */

    /* j must be a power of two */
    if ( (j & (j - 1)) != 0 )
        return (seq_t)0;

    /* Check condition ((i-1) & (j-1)) == (j-1) */
    if ( ( (i - 1) & (j - 1) ) == (j - 1) )
        return (seq_t)1;
    else
        return (seq_t)0;
}
