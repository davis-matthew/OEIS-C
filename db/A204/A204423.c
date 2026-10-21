/*
 * A204423  Infinite matrix: f(i,j)=(2i+j mod 3), by antidiagonals.
 * https://oeis.org/A204423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A204423: an infinite matrix f(i,j) = (2*i + j) mod 3,
 * read along antidiagonals. 
 * For n >= 1: let D be the (smallest) diagonal sum for which
 *               D*(D-1)/2 >= n.  Then let offset = n - (D-1)*(D-2)/2 - 1,
 *               and a(n) = (offset + D + 1) mod 3.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safe guard, though not used */

    /* find smallest D such that D*(D-1)/2 >= n */
    idx_t low = 1, high = 2;
    while (( (__int128)high * (high - 1) / 2 ) < n)
    {
        high += high;               /* double until condition holds */
    }
    while (low < high - 1)
    {
        idx_t mid = low + (high - low) / 2;
        if (( (__int128)mid * (mid - 1) / 2 ) >= n)
            high = mid;
        else
            low = mid;
    }
    idx_t D = high;

    /* cumulative terms before diagonal D */
    idx_t cum_prev = (idx_t)(( (__int128)(D - 1) * (D - 2) ) / 2);

    /* position within the diagonal */
    idx_t offset = n - cum_prev - 1;

    /* value of the sequence at index n */
    seq_t val = (seq_t)((offset + D + 1) % 3);
    return val;
}
