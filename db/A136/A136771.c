/*
 * A136771  n! never ends in this many 0's in base 11.
 * https://oeis.org/A136771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A136771: a(n) is the n-th positive integer that does *not*
 * appear as the exponent of 11 in any factorial n! (i.e., not equal to
 * v_11(k!) for any integer k).  Equivalently, a(n) is the n-th missing
 * value of the sequence v_11(k!) for k = 0,1,2,...
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t misscnt = 0;   /* count of missing exponents found so far   */
    seq_t e = 0;         /* current exponent v_11(m!)                  */
    idx_t m = 1;         /* index of factorial n!                    */

    while (misscnt < n) {
        /* Check if m is divisible by 11.  Only then v_11(m) > 0. */
        if (m % 11 == 0) {
            idx_t temp = m;
            seq_t inc = 0;   /* exponent of 11 in m */
            while (temp % 11 == 0) {
                ++inc;
                temp /= 11;
            }

            if (inc > 1) {
                seq_t new_e = e + inc;
                /* Numbers missing are e+1 .. new_e-1. */
                for (seq_t val = e + 1; val < new_e; ++val) {
                    ++misscnt;
                    if (misscnt == n) {
                        return val;   /* nth missing exponent */
                    }
                }
                e = new_e;
            } else {
                /* inc == 1 → no missing value, just increment e */
                e += 1;
            }
        } else {
            /* m not divisible by 11 → inc == 0, e unchanged */
        }
        ++m;
    }
    /* Should never reach here because there are infinitely many missing exponents. */
    return 0;
}
