/*
 * A212685  Number of (w,x,y,z) with all terms in {1,...,n} and |w-x|=w+|y-z|.
 * https://oeis.org/A212685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-3+3*(-1)^n+4*n-6*n^2+8*n^3)/24
 * Range:     u8 n <= 9, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit signed integer for the intermediate calculation
       to avoid overflow for all practical index values. */
    __int128 N = (unsigned long long)n;          /* promote n to 128 bits */
    __int128 numer = (__int128)8 * N * N * N      /* 8*n³      */
                  - (__int128)6 * N * N          /* -6*n²      */
                  + (__int128)4 * N;             /* +4*n       */
    if (n & 1) numer -= 6;                       /* odd n : -6 */
    return (seq_t)(numer / 24);                  /* divide by 24 */
}
