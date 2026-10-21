/*
 * A082773  Integers n such that there exists at least one divisor d of n, 1 <
 *          d < n, such that d divides n, d+1 divides n+1 and d+2 divides n+2.
 * https://oeis.org/A082773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 9093, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_valid_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the n-th term of OEIS sequence A082773.  
   A(n) is the n-th integer n such that there exists a divisor d of n,
   1 < d < n, with d | n, (d+1) | (n+1) and (d+2) | (n+2).  
   The sequence is 1‑offset: a(1) = 14, a(2) = 26, …  */

/* Standard integer types. */
#include <stdint.h>
#include "../utils/has_valid_divisor.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* The sequence is defined for n >= 1; return 0 for safety. */
        return (seq_t)0;

    uint64_t count = 0;
    uint64_t m = 2;  /* Numbers < 14 never satisfy the condition. */

    while (1) {
        if (has_valid_divisor(m)) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
        ++m;
    }
}
