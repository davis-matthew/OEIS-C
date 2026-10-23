/*
 * A111706  a(n) = concatenation of k times the k-th digit of n.
 * https://oeis.org/A111706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concatenation of k times the k‐th digit of n (digits
 *            counted from least significant, appended in reverse order)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    idx_t temp = n;
    /* maximum digits for a 64–bit integer: 20 */
    seq_t prod[20];
    idx_t m = 0;
    idx_t k = 1;

    /* compute each product digit * position (k) */
    while (temp > 0) {
        seq_t digit = temp % 10;      /* last decimal digit of n          */
        prod[m++] = digit * k;        /* product for current position      */
        temp /= 10;                   /* remove that digit                 */
        ++k;                          /* next position                     */
    }

    /* concatenate the products in reverse order (most significant first) */
    seq_t result = 0;
    for (idx_t i = m; i > 0; --i) {
        seq_t p = prod[i - 1];

        /* count decimal digits of p (p <= 180 for uint64_t inputs) */
        idx_t d = 0;
        seq_t tmp = p;
        do {
            ++d;
            tmp /= 10;
        } while (tmp > 0);

        result = result * pow10(d) + p;
    }

    return result;
}
