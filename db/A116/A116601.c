/*
 * A116601  a(0) = a(1) = 0; for n >= 2, a(n) = floor(sqrt(2^(n-2)-1)).
 * https://oeis.org/A116601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(2^(n-2)-1)), a(0) = a(1) = 0
 * Range:     u8 n <= 18, u16 n <= 34, u32 n <= 66, u64 n <= 130,
 *            u128 n <= 130, big any n
 * Helpers:   _isqrt_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation works for uint64_t, wider unsigned types and
 * arbitrary-precision seq_t classes. It avoids overflow by
 * splitting the computation into cases that handle small exponents
 * with built‑in shifts and larger exponents with big‑int operations
 * or 128‑bit intermediates.                                         */
#include "../utils/pow2.h"
#include "../utils/_isqrt_big.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    idx_t m = n - 2;          /* exponent in 2^(n-2) - 1 */
    /* Even exponent: 2^m - 1 = (2^(m/2) - 1)^2 + 2*(2^(m/2) - 1) */
    if ((m & 1) == 0) {
        idx_t k = m >> 1;
        /* For 64‑bit seq_t, shifts are safe only for k < 64 */
        if (k < (idx_t)sizeof(seq_t) * 8) {
            return ((seq_t)1 << k) - 1;
        } else {
            /* For larger seq_t types (big integer) use pow2 */
            seq_t v = pow2(k);
            v -= 1;
            return v;
        }
    }

    /* Odd exponent: 2^m - 1 = 2 * 2^(m-1) - 1,  floor(sqrt(2^m-1)) = floor(sqrt(2)*2^((m-1)/2)) */
    idx_t k = (m - 1) >> 1;
    /* 64‑bit path: use 128‑bit intermediate */
    if (sizeof(seq_t) <= 8 && k <= 63) {
        unsigned __int128 x = ((unsigned __int128)1 << m) - 1;
        unsigned __int128 lo = 0, hi = ((unsigned __int128)1 << 64);

        while (lo < hi) {
            unsigned __int128 mid = (lo + hi + 1) >> 1;
            unsigned __int128 sq = mid * mid;
            if (sq <= x)
                lo = mid;
            else
                hi = mid - 1;
        }
        return (seq_t)lo;
    }

    /* Big‑int path or 64‑bit with exponent > 127: use arbitrary‑precision sqrt */
    seq_t v = ((seq_t)1 << m) - 1;
    return _isqrt_big(v);
}
