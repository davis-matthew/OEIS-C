/*
 * A176844  The number of iterations of the map n -> n - bigomega(sigma(n))
 *          until reaching 1.
 * https://oeis.org/A176844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   bigomega, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176844:  a(n) = number of iterations of n -> n - bigomega(sigma(n))
 *         until the value reaches 1.
 *  For n = 1, the value is 0.  For n > 1, iterate: n ← n - bigomega(sigma(n))
 *  until n == 1 and count the number of such steps.
 */

#include "../utils/bigomega.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t steps = 0;
    if (n <= 1)          /* a(1) = 0, a(0) is not used */
        return 0;

    while (n > 1) {
        seq_t s = sigma_of(n);                     /* Sum of divisors of n */
        idx_t bw = (idx_t)bigomega((idx_t)s);      /* Total prime‑factor count of s */
        n -= bw;                                   /* Apply the map */
        ++steps;
    }
    return steps;
}
