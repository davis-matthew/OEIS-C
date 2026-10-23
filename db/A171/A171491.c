/*
 * A171491  Natural numbers not divisible by their number of decimal digits.
 * https://oeis.org/A171491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 149, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A171491: natural numbers not divisible by their number of decimal digits.
 * Computed by a linear scan: the nth term is the nth integer that is not divisible
 * by its own digit count.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t found = 0;          /* how many valid numbers have been seen */
    seq_t val = 0;            /* current integer to test */

    while (1) {
        val += 1U;            /* candidate value */

        /* Count decimal digits of val */
        int digits = 0;
        seq_t t = val;
        do {
            digits++;
            t /= (seq_t)10;
        } while (t != 0);

        /* Accept if val is NOT divisible by its digit count */
        if ((val % (seq_t)digits) != (seq_t)0) {
            found += 1U;
            if (found == n) {
                return val;
            }
        }
    }
}
