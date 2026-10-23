/*
 * A189460  n+[ns/r]+[nt/r]; r=2, s=sqrt(5), t=1+sqrt(5).
 * https://oeis.org/A189460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n·√5/2⌋ + ⌊n·(1+√5)/2⌋, r=2, s=√5, t=1+√5
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed with integer arithmetic only, using an   */
/* integer‑sqrt routine for 128‑bit values.                           */
#include <stdint.h>

/* Integer square‑root of a 128‑bit unsigned integer.
   Returns ⌊√x⌋.  Complexity: O(log 2⁶⁴) ≈ 64 iterations. */
static inline unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 low = 0;
    unsigned __int128 high = ((unsigned __int128)1) << 64;  /* 2⁶⁴ > √(2¹²⁸−1) */
    while (low < high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq <= x)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

extern seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n = 0, but return 0 for safety */
        return 0;

    seq_t sn = (seq_t)n;                     /* cast index to seq_t */

    /* Compute floor(n·√5) as ⌊√(5·n²)⌋ using 128‑bit arithmetic. */
    unsigned __int128 prod   = (unsigned __int128)n * n * 5;
    unsigned __int128 sqrt_v = isqrt128(prod);
    seq_t floor_n_sqrt5 = (seq_t)(unsigned long long)sqrt_v;

    /* ⌊n·√5/2⌋  =  floor(n·√5) / 2  (division truncates toward 0) */
    seq_t term1 = floor_n_sqrt5 >> 1;                 /* integer division by 2 */

    /* ⌊n·(1+√5)/2⌋  =  ⌊(n + n·√5)/2⌋  =  ⌊(n + floor(n·√5))/2⌋ */
    seq_t term2 = (sn + floor_n_sqrt5) >> 1;

    return sn + term1 + term2;
}
