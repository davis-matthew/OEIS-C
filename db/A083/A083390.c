/*
 * A083390  Integers m such that 2m + 1 divides lcm(1,3,5,...,2m - 1).
 * https://oeis.org/A083390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 146, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   m_is_good
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A083390:
 * Integers m such that 2m+1 divides lcm(1,3,5,…,2m-1)
 *
 * The routine generates m in increasing order, testing the
 * condition by factoring (2m+1) and verifying that each prime
 * power dividing it does not exceed 2m-1 (otherwise that power
 * cannot be present in the lcm).  Only composite odd numbers
 * can satisfy the condition.
 */
#include <stdbool.h>
#include "../utils/m_is_good.h"

/* External entry point as required by the exercise. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 1;

    while (1) {
        if (m_is_good(m)) {
            count++;
            if (count == n) return (seq_t) m;
        }
        m++;
    }
}
