/*
 * A179867  "Recurrence function" for Thue-Morse infinite word A010060.
 * https://oeis.org/A179867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 3 a(2) = 9 For n ≥ 3: let k = ⌊log₂(n‑2)⌋. Then a(n) =
 *            9·2^k + n – 1 (This follows the definition: 2^k + 2 ≤ n ≤
 *            2^(k+1) + 1)
 * Range:     u8 n <= 33, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned floor_log2_uint64(idx_t v) {
    /* Returns ⌊log₂(v)⌋ for v > 0.  O(64) loop, portable. */
    unsigned k = 0;
    while (v >>= 1) ++k;
    return k;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;

    /* For n ≥ 3 */
    idx_t m = n - 2;                     /* m ≥ 1 */
    unsigned k = floor_log2_uint64(m);    /* k = floor(log₂(n-2)) */

    seq_t pow2 = (seq_t)1;
    for (unsigned i = 0; i < k; ++i) {
        pow2 *= (seq_t)2;                 /* compute 2^k */
    }

    return (seq_t)9 * pow2 + (seq_t)n - (seq_t)1;
}
