/*
 * A175674  a(n) = smallest integer m>=0 such that sum of digits of n and m is
 *          >=10.
 * https://oeis.org/A175674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Series: a(n) = smallest m>=0 such that digit_sum(n)+digit_sum(m)>=10. */
#include "../utils/digit_sum.h"

seq_t a(idx_t n) {
    /* digit_sum returns a seq_t; cast to int for comparison */
    int ds_n = (int)digit_sum((seq_t)n);
    /* If digit sum of n is already at least 10, the answer is 0 */
    if (ds_n >= 10) return (seq_t)0;

    /* Otherwise we need m with digit sum = 10 - ds_n (1..9 or 10) */
    int target = 10 - ds_n;
    if (target <= 9) {
        return (seq_t)target;   /* minimal number is the digit itself */
    } else {                    /* target == 10 -> minimal is 19 */
        return (seq_t)19;
    }
}
