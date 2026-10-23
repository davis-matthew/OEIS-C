/*
 * A130687  Numbers n such that a_1! + a_2! + ... + a_m! is a square number,
 *          where a_1a_2...a_m is the decimal expansion of n.
 * https://oeis.org/A130687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = nth positive integer whose sum of factorials of decimal
 *            digits is a perfect square
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

static const seq_t digit_fact[10] = {
    1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880
};

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static idx_t checked = 1;

    /* Generate terms until we have at least n of them */
    while (seq_len < n) {
        /* Sum of factorials of the decimal digits of 'checked' */
        idx_t tmp = checked;
        seq_t sum = 0;
        if (tmp == 0) {
            sum = digit_fact[0];
        } else {
            while (tmp != 0) {
                int d = (int)(tmp % 10);
                sum += digit_fact[d];
                tmp /= 10;
            }
        }

        /* Check if the sum is a perfect square */
        seq_t root = floor_sqrt(sum);
        if (root * root == sum) {
            if (seq_len >= seq_cap) {
                seq_cap = (seq_cap == 0) ? 16 : seq_cap << 1;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = checked;
        }
        ++checked;
    }

    return seq[n - 1];
}
