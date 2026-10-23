/*
 * A138171  Odd n where d(n) > d(n+1), where d(n) = number of positive
 *          divisors of n.
 * https://oeis.org/A138171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A138171: the nth odd integer n such that d(n) > d(n+1),
   where d(k) is the number of positive divisors of k. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for n < 1, return safely */

    idx_t count = 0;                    /* how many qualifying odds found so far */
    idx_t odd   = 3;                    /* start checking from the first odd ≥3 */

    while (1) {
        uint64_t d1 = divisor_count_u64((uint64_t)odd);
        uint64_t d2 = divisor_count_u64((uint64_t)(odd + 1));

        if (d1 > d2) {
            count++;
            if (count == n) {
                return (seq_t)odd;
            }
        }
        odd += 2;                      /* move to the next odd number */
    }
}
