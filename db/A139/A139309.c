/*
 * A139309  Array by antidiagonals, sum of non-k-gonal numbers between
 *          consecutive k-gonal numbers.
 * https://oeis.org/A139309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is obtained by traversing the 2‑D array T(k,n) =
 *            n*(k-2)*((k-2)*n*n + 1 + 2*n)/2 along antidiagonals, starting at
 *            k = 3, n = 0. The mapping from one‑dimensional index n to array
 *            coordinates (k,n) is: s = floor((sqrt(8*n+1)-1)/2) + 3 //
 *            diagonal index (k + n) prev = s‑1 // number of elements...
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Small indices are handled automatically by the general algorithm. */
    unsigned long long idx = (unsigned long long)n;          // idx fits in 64 bits
    /* Find t0 = floor((sqrt(8*idx+1)-1)/2).  This is the largest integer
       such that t0*(t0+1)/2 <= idx.  */
    double d = sqrt(1.0 + 8.0 * (double)idx);
    unsigned long long t0 = (unsigned long long)((d - 1.0) / 2.0);

    /* Correct possible rounding errors from the floating‑point sqrt.  */
    while (((unsigned __int128)(t0 + 1) * (t0 + 2)) / 2 <= idx)
        ++t0;
    while (((unsigned __int128)t0 * (t0 + 1)) / 2 > idx)
        --t0;

    unsigned long long s      = t0 + 3;                       /* diagonal index k + n */
    unsigned long long prev   = t0 * (t0 + 1) / 2;            /* elements before this diagonal */
    unsigned long long offset = idx - prev;                   /* offset on the diagonal */
    unsigned long long k      = s - offset;                   /* column index */
    unsigned long long m      = offset;                       /* row index (n) */

    if (m == 0)                                            /* first column is always 0 */
        return 0;

    unsigned long long k_minus_2 = k - 2;                    /* k >= 3, so this is safe */
    unsigned __int128 term      = (unsigned __int128)m * k_minus_2;
    unsigned __int128 inside    = (unsigned __int128)k_minus_2 * m * m
                                + 1 + (unsigned __int128)2 * m;
    term = term * inside / 2;
    return (seq_t)term;
}
