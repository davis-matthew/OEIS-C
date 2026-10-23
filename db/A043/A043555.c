/*
 * A043555  Number of runs in base-3 representation of n.
 * https://oeis.org/A043555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of runs (maximal blocks of equal digits) in the
 *            base‑3 representation of n
 * Range:     u8 n <= 9293, u16 n <= 9293, u32 n <= 9293, u64 n <= 9293,
 *            u128 n <= 9293, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                     /* "0" has a single run */

    idx_t   prev = 3;                 /* sentinel value, not a valid trit */
    seq_t   runs = 0;

    while (n != 0) {
        idx_t digit = n % 3;
        if (digit != prev) {          /* start of a new run */
            ++runs;
            prev = digit;
        }
        n /= 3;
    }
    return runs;
}
