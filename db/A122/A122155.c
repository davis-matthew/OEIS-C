/*
 * A122155  Simple involution of natural numbers: List each block of (2^k)-1
 *          numbers (from (2^k)+1 to 2^(k+1) - 1) in reverse order and fix the
 *          powers of 2.
 * https://oeis.org/A122155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; if n is a power of two, a(n)=n; otherwise let k =
 *            floor_log2(n), p=2^k; then a(n)=2^ (k+1) - (n-p)
 * Range:     u8 n <= 255, u16 n <= 9549, u32 n <= 9549, u64 n <= 9549,
 *            u128 n <= 9549, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for zero and powers of two */
    if (n == 0) {
        return 0;
    }

    /* Compute floor_log2(n) using the GCC/Clang intrinsic.
       If unavailable, this fallback uses a simple loop. */
    idx_t k;
#if defined(__GNUC__) || defined(__clang__)
    k = 63 - __builtin_clzll(n);
#else
    k = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        tmp >>= 1;
        ++k;
    }
#endif

    idx_t p = (idx_t)1 << k;   /* 2^k */

    /* If n is exactly a power of two, return n. */
    if (n == p) {
        return (seq_t)n;
    }

    /* For other n, compute a(n) = 2^(k+1) - (n - p).
       Use 128‑bit arithmetic to avoid overflow when k = 63. */
    unsigned __int128 two_p  = (unsigned __int128)p << 1;   /* 2^(k+1) */
    unsigned __int128 offset = (unsigned __int128)(n - p); /* n-p */
    unsigned __int128 result = two_p - offset;

    return (seq_t)result;
}
