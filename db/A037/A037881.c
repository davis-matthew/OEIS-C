/*
 * A037881  (1/2)*Sum{|d(i)-e(i)|} where Sum{d(i)*4^i} is base 4
 *          representation of n and e(i) are digits d(i) in nondecreasing
 *          order.
 * https://oeis.org/A037881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (1/2) * Σ |d_i - e_i|, where d_i are the base‑4 digits of
 *            n (least significant first) and e_i are the same digits sorted
 *            in nondecreasing order
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 (outside the official offset). */
    if (n == (idx_t)0) return (seq_t)0;

    /* Extract base‑4 digits of n.  The maximum number of digits for a
       64‑bit integer is 32; 64 is safely over‑provisioned. */
    uint8_t digits[64];
    size_t len = 0;
    idx_t temp = n;
    while (temp > (idx_t)0) {
        digits[len++] = (uint8_t)(temp & 3U);   /* temp % 4 */
        temp >>= 2;                            /* temp /= 4 */
    }

    /* Count the frequency of each possible digit (0..3). */
    uint8_t count[4] = {0, 0, 0, 0};
    for (size_t i = 0; i < len; ++i) {
        count[digits[i]]++;
    }

    /* Reconstruct the sorted digits in e[]. */
    uint8_t sorted[64];
    size_t idx = 0;
    for (uint8_t d = 0; d < 4; ++d) {
        for (uint8_t c = 0; c < count[d]; ++c) {
            sorted[idx++] = d;
        }
    }

    /* Compute the L1 distance between the original and sorted digit
       lists, and return half of the result.  The sum is always even
       because both lists contain the same multiset of digits. */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < len; ++i) {
        seq_t diff = digits[i] > sorted[i]
                     ? (seq_t)(digits[i] - sorted[i])
                     : (seq_t)(sorted[i] - digits[i]);
        sum += diff;
    }

    return sum / (seq_t)2;
}
