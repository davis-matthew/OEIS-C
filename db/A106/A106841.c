/*
 * A106841  Numbers m such that m, m+1 and m+2 have odd part of the form
 *          4*k+1.
 * https://oeis.org/A106841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer m such that the odd part of m, m+1 and
 *            m+2 are all congruent to 1 modulo 4
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   odd_part_congruent1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm scans integers m=1,2,3,…, counting how many satisfy the
   condition.  When the nth such integer is found it is returned as a
   value of type seq_t.  The routine is O(n) and works for any idx_t, seq_t
   combination that the build supplies. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/odd_part_congruent1.h"

seq_t a(idx_t n)
{
    if (n == 0)      /* n is always ≥ 1 in the OEIS call set, but guard. */
        return 0;

    idx_t m = 1;          /* candidate m */
    idx_t count = 0;      /* how many valid m's found */

    while (1) {
        /* Check triple (m, m+1, m+2).  Convert to seq_t before
           calling odd_part_congruent1. */
        if (   odd_part_congruent1((seq_t)m)
            && odd_part_congruent1((seq_t)(m + 1))
            && odd_part_congruent1((seq_t)(m + 2)))
        {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
        ++m;
    }
}
