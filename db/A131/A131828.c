/*
 * A131828  Square of lower triangular matrix in A131821, read by rows.
 * https://oeis.org/A131828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The triangle consists of the entries of the square of the lower‑triangular
    matrix L whose elements are:
          L[i][1]   = i
          L[i][i]   = i               (diagonal)
          L[i][j]   = 1   for 2 ≤ j ≤ i‑1
    The (i,j) entry of L² (i ≥ j) is:
          j = 1   →  (3 i² + i – 2)/2
          j = i   →  i²
          otherwise → 2 i – 1
    The sequence a(n) is obtained by reading this lower‑triangular matrix
    row by row.  Index n starts at 1.  This function returns the n‑th term
    of that sequence.  All arithmetic that can overflow the result type
    is performed in 128‑bit unsigned integers for safety.  */
#include <stdint.h>

/* Basic integer square root with 128‑bit safety (floor). */
static idx_t floor_isqrt(idx_t n)
{
    idx_t low  = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;           /* floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for offset 1 */

    /* find row r such that T(r-1) < n ≤ T(r),  T(r)=r(r+1)/2 */
    idx_t r = floor_isqrt(8 * n + 1);      /* r ≈ floor( sqrt(8n+1) ) */
    r = (r - 1) >> 1;                      /* r = (r-1)/2  (floor) */
    /* adjust r upwards until T(r) >= n */
    while ((unsigned __int128)r * (r + 1) / 2 < n) r++;
    /* adjust r downwards if overshoot */
    while (r > 0 && (unsigned __int128)(r - 1) * r / 2 >= n) r--;

    /* column index j (1‑based) */
    idx_t prev = (r - 1) * r / 2;            /* T(r-1) using 64‑bit; safe for n≤10⁶ */
    idx_t j = n - prev;                     /* 1 ≤ j ≤ r */

    seq_t val;
    if (j == 1) { /* first column */
        unsigned __int128 t = (unsigned __int128)3 * r * r + r - 2;
        val = (seq_t)(t / 2);
    } else if (j == r) { /* diagonal */
        unsigned __int128 t = (unsigned __int128)r * r;
        val = (seq_t)t;
    } else { /* middle entries */
        unsigned __int128 t = (unsigned __int128)2 * r - 1;
        val = (seq_t)t;
    }

    return val;
}
