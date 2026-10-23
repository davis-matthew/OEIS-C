/*
 * A036416  Number of empty intervals when fractional_part(i*Pi) for i = 1,
 *          ..., n is plotted along [ 0, 1 ] subdivided into n equal regions.
 * https://oeis.org/A036416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 371, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A036416: number of empty intervals when plotting fractional_part(i*π),
 *  i = 1..n, on [0,1] subdivided into n equal parts.
 *  a(n) = n - #(unique buckets occupied by iπ mod 1, i=1..n)   */

/*  Using only integer arithmetic.
 *  π is scaled by 10^30: PI_SCALE = floor(π * 10^30).
 *  SCALE  = 10^30.  All computations are done in unsigned __int128
 *  to avoid overflow for n ≤ 10 000.                                   */

#include <stdlib.h>
#include <stdint.h>

/*  π * 10^30  (31 decimal digits)  */
static const unsigned __int128 PI_SCALE =
    ((unsigned __int128)314159265358979323ULL) * 10000000000000ULL + 8462643383279ULL;

/*  SCALE = 10^30 = 10^18 * 10^12  */
static const unsigned __int128 SCALE =
    ((unsigned __int128)1000000000000000000ULL) * 1000000000000ULL;

/*  Return the zero‑based bucket index for iπ mod 1
 *  bucket = floor( n * frac(iπ) ) = floor(n*iπ) - n*floor(iπ)   */
static inline idx_t bucket_idx(idx_t i, idx_t n)
{
    unsigned __int128 scaled_i = (unsigned __int128)i * PI_SCALE;
    unsigned __int128 i_pi    = scaled_i / SCALE;        /* floor(iπ)        */
    unsigned __int128 scaled_in = scaled_i * n;          /* i*n*PI_SCALE     */
    unsigned __int128 i_n_pi   = scaled_in / SCALE;      /* floor(i*nπ)      */
    unsigned __int128 prod = i_pi * (unsigned __int128)n;
    unsigned __int128 k    = i_n_pi - prod;              /* bucket index     */
    return (idx_t)k;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    unsigned char *occupied = (unsigned char *)calloc((size_t)n, 1);
    if (occupied == NULL)
        return (seq_t)0;          /* allocation failure, return 0 */

    idx_t occupied_cnt = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t k = bucket_idx(i, n);
        if (!occupied[k]) {
            occupied[k] = 1;
            ++occupied_cnt;
        }
    }
    free(occupied);
    return (seq_t)(n - occupied_cnt);
}
