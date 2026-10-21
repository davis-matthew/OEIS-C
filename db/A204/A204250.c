/*
 * A204250  Symmetric matrix read by antidiagonals given by T(i,j)=i*j+i+j-2.
 * https://oeis.org/A204250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = i*j + i + j - 2, where (i,j) is the position of n on the
 *            * (1‑based) antidiagonal read of the symmetric matrix T(i,j) =
 *            i*j + i + j - 2. * The function maps the index n (n ≥ 1) to that
 *            matrix entry using only integer * arithmetic
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A204250  */

#include <stdint.h>

/* Local routine: floor(sqrt(x)) for a 128‑bit unsigned integer x.
 * Binary search is used, with a 64‑bit search space.  All arithmetic is
 * performed in 128 bits to avoid overflow. */
static inline idx_t floor_sqrt_u128(unsigned __int128 x)
{
    idx_t low  = 0;
    idx_t high = (idx_t)-1;        /* 64‑bit maximum */

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (__int128)mid * mid;

        if (sq <= x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* undefined for n = 0 */

    /* Find t such that t*(t+1)/2 ≥ n and (t-1)*t/2 < n.  The integer
     * t is the length of the antidiagonal containing the nth element. */
    unsigned __int128 disc = (unsigned __int128)8 * n + 1;        /* 8n + 1 */
    idx_t sqrt_d = floor_sqrt_u128(disc);
    idx_t t = (sqrt_d - 1) / 2;                                 /* floor */

    /* Correct possible off‑by‑one due to the floor operation */
    if (((unsigned __int128)t * (t + 1)) / 2 < n) t++;

    /* Offset within the t‑th antidiagonal (1‑based) */
    idx_t prev_tri = (t - 1) * t / 2;          /* (t-1)*t/2 */
    idx_t offset   = n - prev_tri;             /* 1 ≤ offset ≤ t */

    /* Coordinates: i = offset, j = (t+1)-offset */
    idx_t i = offset;
    idx_t j = (t + 1) - offset;

    /* Matrix entry value */
    return (seq_t)i * j + i + j - 2;
}
