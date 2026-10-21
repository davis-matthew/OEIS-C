/*
 * A079320  CATS sequence: cube-add-then-sort variation of RATS (reverse, add
 *          then sort) sequence.
 * https://oeis.org/A079320
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sort_digits(n^3 + n)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 2067, u64 n <= 9472,
 *            u128 n <= 9472, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* compute s = n^3 + n  as seq_t */
    seq_t n_seq = (seq_t)n;
    seq_t s = n_seq * (n_seq * n_seq + 1);

    /* count decimal digits of s */
    int count[10] = {0};
    seq_t zero = 0;
    seq_t tmp = s;
    while (tmp > zero) {
        int d = (int)(tmp % 10);   /* d in [0,9] */
        ++count[d];
        tmp = tmp / 10;
    }

    /* rebuild the smallest number with digits sorted ascending,
       ignoring leading zeros */
    seq_t result = 0;
    bool started = false;
    for (int d = 0; d <= 9; ++d) {
        for (int i = 0; i < count[d]; ++i) {
            if (!started) {
                if (d != 0) {
                    result = d;
                    started = true;
                }
                /* skip leading zeros */
            } else {
                result = result * 10 + d;
            }
        }
    }
    return result;
}
