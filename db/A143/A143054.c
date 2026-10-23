/*
 * A143054  A143052(A143052(n)).
 * https://oeis.org/A143054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a is a permutation formed by cyclically shifting each block of
 *            consecutive odd length 1,1,3,5,7,9,… left by 2 positions. Block
 *            indices: b = 0 → start 0, length 1 b ≥ 1 → start (b-1)²+1, end
 *            b², length 2b–1 For n ≥ 1, let b = ceil_sqrt(n): if n < b² - 1 →
 *            a(n) = n + 2 else if n == b²-2 → a(n) = st...
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   ceil_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ceil_isqrt.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;           /* special block of length 1 */

    idx_t b = ceil_isqrt(n);        /* block index (b ≥ 1 for n ≥ 1) */
    if (b == 1)                     /* block of length 1 (n == 1) */
        return n;

    /* block start and length */
    idx_t start = (b - 1) * (b - 1) + 1;   /* (b-1)^2 + 1 */
    idx_t L     = 2 * b - 1;               /* odd length */

    idx_t offset = n - start;              /* 0‑based offset within block */

    /* apply the cyclic shift by 2 left */
    if (offset < L - 2)
        return (seq_t)n + 2;               /* first L-2 elements shift forward */
    else if (offset == L - 2)
        return (seq_t)start;               /* wrap‑around to block start */
    else
        return (seq_t)(start + 1);         /* wrap‑around to block start+1 */
}
