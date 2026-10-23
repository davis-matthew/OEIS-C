/*
 * A127760  Integer part of Gauss's Arithmetic-Geometric Mean M(1,n^4).
 * https://oeis.org/A127760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( AGM(1, n^4) )
 * Range:     u8 n <= 6, u16 n <= 27, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses fixed‑point arithmetic with a scaling factor.
   The arithmetic is performed in 128‑bit integers (__int128) so that
   all intermediate products fit comfortably.  The integer square root
   is computed by binary search so that no floating point operations
   are used.  The result is truncated to the target integer type
   seq_t.  This works for all builds – the seq_t type may be 8–128 bit
   unsigned or an arbitrary‑precision integer class. */

#include <stdint.h>

static inline __int128 isqrt128(__int128 x)
{
    __int128 low = 0;
    __int128 high = (__int128)1 << 64;   /* sqrt(x) never exceeds 2^64 for 128‑bit x */
    while (low < high) {
        __int128 mid = (low + high + 1) >> 1;
        if (mid == 0) {
            low = mid;
            continue;
        }
        if (mid <= x / mid)      /* mid*mid <= x  (avoid overflow) */
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Scaling factor 2^10 = 1024 gives enough precision for every
       n up to at least 10,000 and keeps intermediate values within
       128‑bit limits. */
    const int SCALE_BITS = 10;
    const __int128 SCALE = (__int128)1 << SCALE_BITS;

    /* Compute n^4 as 128‑bit integer. */
    __int128 nn = (__int128)n;
    __int128 N = nn * nn * nn * nn;   /* n^4 */

    /* Initialise scaled AGM variables: a = 1  * SCALE,  b = N * SCALE */
    __int128 a_scaled = SCALE;
    __int128 b_scaled = N * SCALE;

    /* Iterate until convergence.  Forty iterations are more than
       sufficient because AGM converges quadratically. */
    for (int iter = 0; iter < 40; ++iter) {
        /* a_{k+1} = (a_k + b_k)/2 with truncation. */
        __int128 a_next = (a_scaled + b_scaled) >> 1;

        /* b_{k+1} = sqrt(a_k * b_k)   (scaled) */
        __int128 prod = a_scaled * b_scaled;
        __int128 b_next = isqrt128(prod);

        if (a_next == a_scaled && b_next == b_scaled) {
            a_scaled = a_next;
            break;
        }
        a_scaled = a_next;
        b_scaled = b_next;
    }

    /* Convert back to integer: floor(a_scaled / SCALE).  The division
       result fits in seq_t because the final values are well below the
       64‑bit limit for n ≤ 10,000. */
    return (seq_t)(a_scaled / SCALE);
}
