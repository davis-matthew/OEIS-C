/*
 * A185592  a(n) = floor(n^(3/2))*floor(1 + n^(3/2))*floor(2 + n^(3/2))/6.
 * https://oeis.org/A185592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n^(3/2)⌋ ⋅ ⌊1 + n^(3/2)⌋ ⋅ ⌊2 + n^(3/2)⌋ / 6 which
 *            simplifies to a(n) = m⋅(m+1)⋅(m+2)/6 where m = ⌊n^(3/2)⌋
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 204, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for an unsigned 64‑bit value.
   Returns ⌊√x⌋.  The implementation uses a binary search
   with 128‑bit intermediate multiplication to avoid overflow. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = 1ULL << 32;   /* √(2^64) < 2^32 */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Computes the n-th term of the OEIS sequence A185592. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* for safety, though n ≥ 1 per the offset */

    /* Compute ⌊n^(3/2)⌋ as ⌊√(n³)⌋. */
    idx_t cube = (unsigned long long)n * n * n;   /* n ≤ 10⁴ → cube ≤ 10¹² */
    idx_t m_raw = isqrt_u64(cube);
    seq_t m = (seq_t)m_raw;  /* cast to the sequence type */

    /* Compute m*(m+1)*(m+2)/6 using seq_t arithmetic. */
    seq_t result = m;
    result *= (m + 1);
    result *= (m + 2);
    result /= 6;

    return result;
}
