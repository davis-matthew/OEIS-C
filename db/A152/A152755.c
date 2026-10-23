/*
 * A152755  Triangle read by rows: row n gives the vector b[n] where b[0] =
 *          {-1}; b[1] = {1, -1}; b[n] = prime[n + 2 ], -prime[n + 2] + n - 1,
 *          -1,-1, ..., -1.
 * https://oeis.org/A152755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 464, i16 n <= 1325, i32 n <= 1325, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A152755
   Triangle read by rows: row n gives b[n] where
   b[0]   = {-1};
   b[1]   = { 1, -1};
   b[n]   = { prime[n+2],   -prime[n+2] + n - 1,   -1, -1, ..., -1 }   (n ≥ 2)
   Offset: 0
   Implementation uses integer binary search to locate the row,
   and get_prime(k) to obtain the (k)-th prime (1‑based). */

#include "../utils/get_prime.h"   /* provides uint32_t get_prime(idx_t k) */

seq_t a(idx_t n)
{
    /* --- locate the row r containing index n --- */
    idx_t low = 0;
    idx_t high = 1;

    /* increase high until triangular(high) > n */
    while ( (unsigned __int128)high * (high + 1) / 2 <= n )
    {
        if (high > (idx_t)0x8000000000000000ULL) /* avoid overflow */
            break;
        high <<= 1;
    }

    /* binary search for largest r with r*(r+1)/2 <= n */
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low - 1;   /* r = row number */

    /* position of n within its row */
    idx_t pos = n - (unsigned __int128)r * (r + 1) / 2;

    /* --- compute the value --- */
    if (r == 0)
    {
        /* row 0 */
        return (seq_t)-1;
    }
    else if (r == 1)
    {
        /* row 1: {1, -1} */
        return (pos == 0) ? (seq_t)1 : (seq_t)-1;
    }
    else
    {
        /* row r ≥ 2 */
        seq_t p = (seq_t)get_prime(r + 2);   /* first element is prime[r+2] */

        if (pos == 0)
            return p;
        else if (pos == 1)
        {
            seq_t r_seq = (seq_t)r;
            return -p + r_seq - 1;
        }
        else
        {
            return (seq_t)-1;
        }
    }
}
