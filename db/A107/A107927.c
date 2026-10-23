/*
 * A107927  Smallest number m>n such that some digit of n and some digit of m
 *          sum up to 9.
 * https://oeis.org/A107927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the smallest m > n such that some digit of n and some
 *            digit of m sum to 9
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   has_comp_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_comp_digit.h"

seq_t a(idx_t n) {
    /* determine which digits occur in n */
    bool have[10] = { false };
    seq_t tn = (seq_t)n;
    if (tn == 0) {
        have[0] = true;
    } else {
        while (tn != 0) {
            int d = (int)(tn % 10);
            have[d] = true;
            tn /= 10;
        }
    }

    /* search for the smallest m > n that satisfies the condition */
    for (idx_t m = n + 1; ; ++m) {
        seq_t vm = (seq_t)m;
        if (has_comp_digit(vm, have)) {
            return vm;
        }
    }
}
