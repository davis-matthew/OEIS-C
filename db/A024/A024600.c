/*
 * A024600  s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n+1-k), where k = [ (n+1)/2 ],
 *          s = (odd natural numbers), t = A001950 (upper Wythoff sequence).
 * https://oeis.org/A024600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} (2i−1) · t(n+1−i) , where t(m) = m +
 *            ⌊(m + ⌊m·√5⌋)/2⌋ (upper Wythoff sequence)
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-01)
 * Helpers:   isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u128.h"

/* t(m) for any m ≥ 1 – using 128‑bit arithmetic only */
static inline unsigned __int128 compute_t128(idx_t m)
{
    unsigned __int128 mm   = (unsigned __int128)m;
    unsigned __int128 sq   = isqrt_u128((unsigned __int128)5 * mm * mm);
    unsigned __int128 phi  = (mm + sq) / 2;   /* floor(m·φ) */
    return mm + phi;                         /* t(m) = m + floor(m·φ) */
}

/* Public function – sequence A024600 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, safety only */

    unsigned __int128 sum = 0;
    idx_t k = (n + 1) / 2;                /* floor((n+1)/2) */

    for (idx_t i = 1; i <= k; ++i) {
        unsigned __int128 s = (unsigned __int128)2 * (unsigned __int128)i - 1;
        idx_t m = n + 1 - i;              /* argument for t() */
        unsigned __int128 t = compute_t128(m);
        sum += s * t;
    }
    return (seq_t)sum;
}
