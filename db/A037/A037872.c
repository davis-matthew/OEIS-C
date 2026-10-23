/*
 * A037872  (1/2)*Sum{|d(i)-e(i)|}, where Sum{d(i)*4^i} is base 4
 *          representation of n and e(i) are digits d(i) in nonincreasing
 *          order, for i=0,1,...,m.
 * https://oeis.org/A037872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (1/2) * Sum_{i=0}^m |d(i)-e(i)|, where d(i) are the
 *            base‑4 digits of n (least significant first) and e(i) are those
 *            digits sorted in non‑increasing order
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
        /* Extract base‑4 digits of n (least significant first). */
        uint8_t digits[64];
        uint8_t sorted[64];
        idx_t len = 0;
        idx_t tmp = n;

        if (tmp == 0) {
                digits[0] = 0;
                len = 1;
        } else {
                while (tmp > 0) {
                        digits[len++] = (uint8_t)(tmp & 3ULL); /* tmp % 4 */
                        tmp >>= 2;                               /* tmp /= 4 */
                }
        }

        /* Copy to sorted array and sort in descending order. */
        for (idx_t i = 0; i < len; ++i) {
                sorted[i] = digits[i];
        }
        for (idx_t i = 1; i < len; ++i) {
                uint8_t key = sorted[i];
                int j = (int)i - 1;
                while (j >= 0 && sorted[j] < key) {
                        sorted[j + 1] = sorted[j];
                        --j;
                }
                sorted[j + 1] = key;
        }

        /* Compute sum of absolute differences and divide by 2. */
        seq_t sum = 0;
        for (idx_t i = 0; i < len; ++i) {
                seq_t diff = (digits[i] > sorted[i]) ?
                             (digits[i] - sorted[i]) :
                             (sorted[i] - digits[i]);
                sum += diff;
        }
        return sum / (seq_t)2;
}
