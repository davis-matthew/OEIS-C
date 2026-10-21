/*
 * A076068  Smallest number that can be formed by using the nonzero digits of
 *          the numbers 1+n(n-1)/2 through n(n+1)/2.
 * https://oeis.org/A076068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest number that can be formed by using the *
 *            non‑zero decimal digits of all integers from 1 + n(n‑1)/2 up to
 *            * n(n+1)/2, inclusive. The digits are arranged in ascending
 *            order
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Boundaries of the triangular range L … R. */
    idx_t L = 1 + n * (n - 1) / 2;
    idx_t R = n * (n + 1) / 2;

    /* Count frequencies of non‑zero digits 1 … 9. */
    uint32_t freq[10] = {0};
    for (idx_t x = L; x <= R; ++x) {
        idx_t t = x;
        while (t > 0) {
            uint8_t d = t % 10;
            if (d != 0) {
                ++freq[d];
            }
            t /= 10;
        }
    }

    /* Build the smallest number by concatenating digits in ascending
       order.  Since seq_t supports addition and multiplication by
       integers, we can construct the result incrementally. */
    seq_t result = 0;
    for (uint8_t d = 1; d <= 9; ++d) {
        uint32_t k = freq[d];
        while (k--) {
            result = result * 10 + d;
        }
    }

    return result;
}
