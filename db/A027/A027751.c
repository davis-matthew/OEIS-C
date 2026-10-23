/*
 * A027751  Irregular triangle read by rows in which row n lists the proper
 *          divisors of n (those divisors of n which are < n), with the first
 *          row {1} by convention.
 * https://oeis.org/A027751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) is the n-th entry in the concatenation of the proper *
 *            divisors of m (row 1 is {1}). The routine scans rows until it *
 *            finds the desired term
 * Range:     u8 n <= 2768, u16 n <= 9734, u32 n <= 9734, u64 n <= 9734,
 *            u128 n <= 9734, big any n
 * Helpers:   proper_divisor_at, proper_divisor_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/proper_divisor_at.h"
#include "../utils/proper_divisor_count.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n < 1 */
    idx_t m = 1;
    size_t cumulative = 0;                   /* terms counted so far */

    while (1) {
        size_t count;
        if (m == 1)
            count = 1;                        /* row 1 contains {1} */
        else
            count = proper_divisor_count(m);

        if (cumulative + count >= (size_t)n) {
            size_t offset = (size_t)n - cumulative;   /* 1‑based in the row */
            return proper_divisor_at(m, offset);
        }

        cumulative += count;
        ++m;
    }
}
