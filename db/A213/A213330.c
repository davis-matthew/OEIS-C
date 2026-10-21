/*
 * A213330  Minimum deviation from n in Collatz trajectory of n.
 * https://oeis.org/A213330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 7726, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimum deviation from n in Collatz trajectory of n */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)0;

    seq_t seq_n = (seq_t)n;
    seq_t cur = seq_n;
    seq_t min_diff = (seq_t)-1;
    int first = 1;

    while (cur != (seq_t)1) {
        if ((cur & (seq_t)1) == 0) {      /* even */
            cur = cur >> 1;
        } else {                          /* odd */
            cur = 3 * cur + 1;
        }

        seq_t diff = (cur > seq_n) ? (cur - seq_n) : (seq_n - cur);
        if (first || diff < min_diff) {
            min_diff = diff;
            first = 0;
        }
    }

    return min_diff;
}
