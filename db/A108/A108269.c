/*
 * A108269  Numbers of the form (2*m - 1)*4^k where m >= 1, k >= 1.
 * https://oeis.org/A108269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth positive integer whose binary representation ends
 *            in an even (positive) number of zeros. In other words, the
 *            exponent of 2 in n is even and at least 2. This is equivalent to
 *            numbers of the form (2*m-1)*4^k with m>=1, k>=1
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   good_term, v2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/good_term.h"
#include "../utils/v2.h"

/* Return the n-th term of sequence A108269 (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n=0, return 0 as a placeholder */

    seq_t val = 4;         /* first term is 4 */
    idx_t  count = 1;      /* how many terms found so far */

    while (count < n) {
        /* search for the next good term */
        do {
            val++;
        } while (!good_term(val));
        count++;
    }
    return val;
}
