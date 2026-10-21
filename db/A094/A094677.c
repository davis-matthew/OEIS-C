/*
 * A094677  Sum of digits is divisible by 10.
 * https://oeis.org/A094677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 6553, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A094677 – numbers whose sum of decimal digits is divisible by 10
 * 
 * Formula: a(n) = 10*n + r(n) where
 *   r(n) = (10 - (sum_digits(n) mod 10)) mod 10
 * 
 * This follows from the observation that the last digit of 10*n is 0.
 * Adding r(n) (≤9) does not create a carry, so the digit sum of
 * 10*n + r(n) equals sum_digits(n) + r(n), which is an exact multiple
 * of 10 by construction.  
 * The sequence is sorted in increasing order because 10*n is strictly
 * increasing and the added r(n) is bounded by 9.
 * 
 * Complexity: O(log10 n) time, O(1) space.
 */

#include <stdint.h>

static inline int digit_sum_idx(idx_t x)
{
    int s = 0;
    while (x) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

seq_t a(idx_t n)
{
    /* convert n to seq_t for arithmetic */
    seq_t sn = (seq_t)n;

    /* digit sum of n (as idx_t) */
    int s = digit_sum_idx(n);
    int r = (10 - (s % 10)) % 10;  /* remainder to reach multiple of 10 */

    /* a(n) = 10*n + r */
    return sn * (seq_t)10 + (seq_t)r;
}
