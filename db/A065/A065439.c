/*
 * A065439  Numbers n such that s[n], p[n] and s[n]+p[n] are all prime
 *          numbers, where s[n] is the sum of digits of n and p[n] is the
 *          product of digits of n.
 * https://oeis.org/A065439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 32, u128 n <= 60,
 *            big any n
 * Helpers:   build_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A065439 – numbers consisting of a single digit 2 and
 *  remaining digits 1, such that the sum of digits (m+2) and the
 *  sum plus the product (m+4) are prime.
 *  The term a(n) (n ≥ 1) is the n‑th such number in ascending
 *  numeric order.  m is the count of 1‑digits (m ≥ 1).
 *  For a given m, the numbers are the (m+1) permutations of
 *  one 2 and m ones, listed with the 2 moving from the least
 *  significant position to the most significant one. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_value.h"

/* test whether x is prime – only values up to a few thousand are needed */
static bool is_prime(idx_t x) {
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (idx_t i = 3; i <= x / i; i += 2) {
        if (x % i == 0) return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* n is 1‑based; for safety return 0 for n==0, though that
     * value is outside the defined offset. */
    if (n == 0) return 0;

    idx_t left = n - 1;   /* 0‑based rank within the infinite list   */

    for (idx_t m = 1; ; ++m) {               /* m = number of 1‑digits   */
        idx_t sum = m + 2;                    /* s[n]                      */
        if (is_prime(sum) && is_prime(sum + 2)) {  /* twin‑prime test   */
            idx_t group = m + 1;              /* number of permutations   */
            if (left < group) {
                /* ascending numeric order: 2 moves from least
                 * significant to most significant. */
                idx_t pos = m - left;        /* position of the 2 from the left */
                return build_value(m, pos);
            }
            left -= group;                   /* skip this block           */
        }
    }
}
