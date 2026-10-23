/*
 * A031293  Position of n-th 6 in A007376.
 * https://oeis.org/A031293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, base
 * Formula:   a(n) = position of the n‑th occurrence of digit 6 in the decimal
 *            concatenation of positive integers (123456789101112...)
 * Range:     u8 n <= 22, u16 n <= 5563, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t pos = 0;          /* 1‑based position in the concatenated string */
    idx_t count = 0;        /* number of 6's seen so far */

    for (idx_t x = 1; ; ++x) {
        /* Gather the decimal digits of x into a small buffer */
        idx_t tmp = x;
        int digits[20];     /* enough for any 64‑bit integer */
        int len = 0;
        do {
            digits[len++] = (int)(tmp % 10);
            tmp /= 10;
        } while (tmp);

        /* Process digits from most significant to least */
        for (int i = len - 1; i >= 0; --i) {
            pos++;                     /* advance position */
            if (digits[i] == 6) {      /* found a 6 */
                count++;
                if (count == n)         /* reached the n‑th 6 */
                    return pos;
            }
        }
    }
}
