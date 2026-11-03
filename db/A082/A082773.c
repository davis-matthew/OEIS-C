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
 * Validated: standard, pass, 70 terms through n = 7893 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the n-th term of OEIS sequence A082773.  
   A(n) is the n-th integer n such that there exists a divisor d of n,
   1 < d < n, with d | n, (d+1) | (n+1) and (d+2) | (n+2).  
   The sequence is 1‑offset: a(1) = 14, a(2) = 26, …  */

/* Standard integer types. */
#include <stdint.h>

static int has_valid_divisor(uint64_t m)
{
    /* Check all divisors d of m, with 1 < d < m.  */
    for (uint64_t d = 2; d <= m / d; ++d) {
        if (m % d == 0) {
            /* d is a proper divisor.  */
            if (((m + 1) % (d + 1) == 0) && ((m + 2) % (d + 2) == 0))
                return 1;
            /* Also check the complementary divisor m/d, if distinct. */
            uint64_t d2 = m / d;
            if (d2 != d && d2 > 1 && d2 < m) {
                if (((m + 1) % (d2 + 1) == 0) && ((m + 2) % (d2 + 2) == 0))
                    return 1;
            }
        }
    }
    return 0;
}

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
