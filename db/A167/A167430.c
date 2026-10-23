/*
 * A167430  Fractal sequence of the interspersion A163255.
 * https://oeis.org/A167430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = if pos <= k then 2*pos-1 else 2*(pos-k), where k is the
 *            smallest integer with k*(k+1) >= n and pos = n - (k-1)*k. This
 *            yields the fractal sequence of interspersion A163255 as
 *            described in OEIS A167430
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row k that contains index n:
       k is the smallest integer with k*(k+1) >= n. */
    idx_t low  = 1;
    idx_t high = 1;
    /* Double high until it is large enough */
    while ((unsigned __int128)high * (high + 1) < (unsigned __int128)n)
        high <<= 1;
    /* Binary search for the smallest k */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) >= (unsigned __int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t k = low;                           /* row number */
    /* Compute position inside the row */
    unsigned __int128 prev_len = (unsigned __int128)(k - 1) * k;
    idx_t pos = (idx_t)((unsigned __int128)n - prev_len); /* 1 <= pos <= 2k */

    if (pos <= k)
        return (seq_t)(2 * (seq_t)pos - (seq_t)1);         /* odd part */
    else
        return (seq_t)(2 * (seq_t)(pos - k));             /* even part */
}
