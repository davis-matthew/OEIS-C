/*
 * A115795  Integers k such that 2*k XOR 9*k = 11*k.
 * https://oeis.org/A115795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 742, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A115795: integers k such that 2*k XOR 9*k = 11*k.
 * The condition is equivalent to (2*k) & (9*k) == 0.
 * We iterate k from 0 upwards, counting those that satisfy the
 * condition, and return the n-th such value.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k     = 0;           /* use idx_t for the iteration so that
                                 multiplication does not overflow  */
    while (1) {
        /* test condition: (2*k) XOR (9*k) == 11*k  <=>  (2*k & 9*k) == 0 */
        if (((k << 1) & (9 * k)) == 0) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
