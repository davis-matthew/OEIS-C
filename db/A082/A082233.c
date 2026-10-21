/*
 * A082233  Square array T(n,k) = 2*n + k, read by antidiagonals in a zigzag
 *          fashion, n >= 0 and k >= 1.
 * https://oeis.org/A082233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 5049, u16 n <= 5049, u32 n <= 5049, u64 n <= 5049,
 *            u128 n <= 5049, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code implements A082233.  The m‑th term a(m) is obtained by
   mapping the index m to its antidiagonal in the square array
   T(n,k) = 2*n + k (n ≥ 0, k ≥ 1), traversed in a zig‑zag order. */

/* Standard types used in the build.   Do not modify or remove. */
#include <stdint.h>
#include <limits.h>
#include <stdlib.h>

/* Integer square root for values up to 2^128‑1.  Pure 128‑bit
   arithmetic guarantees correctness for all possible indices. */
static uint64_t isqrt_u128(unsigned __int128 v)
{
    uint64_t low = 0, high = UINT64_MAX;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= v)
            low = mid + 1;
        else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return low - 1;
}

/* Return the m‑th term a(m) of A082233. */
seq_t a(idx_t m)
{
    uint64_t u = (uint64_t)m;                     /* ``m'' fits 64‑bit */

    /* Compute s = floor((1 + sqrt(1 + 8*m)) / 2). */
    unsigned __int128 val = (unsigned __int128)8 * u + 1;
    uint64_t sqrt_val = isqrt_u128(val);
    uint64_t s = (sqrt_val + 1) / 2;          /* anti‑diagonal number, s ≥ 1 */

    /* Elements before diagonal s: (s‑1)*s/2. */
    unsigned __int128 before128 = (unsigned __int128)(s - 1) * s / 2;
    uint64_t before = (uint64_t)before128;

    uint64_t t = u - before;                  /* offset inside the diagonal (0‑based) */

    uint64_t n, k;
    if ((s & 1ULL) == 0) {                    /* even s ⇒ ascending n */
        n = t;
        k = s - t;
    } else {                                  /* odd s ⇒ descending n */
        n = s - 1 - t;
        k = t + 1;
    }

    /* T(n,k) = 2*n + k.  Cast back to seq_t. */
    return (seq_t)((seq_t)2 * (seq_t)n + (seq_t)k);
}
