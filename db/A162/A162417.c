/*
 * A162417  Find max {primes such that p < n^2, n = 2,3,...}, then the gap
 *          g(n) between that prime and its successor. This sequence is the
 *          sequence of differences {2n - g(n)}.
 * https://oeis.org/A162417
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n - g(n), where g(n) is the gap between the largest
 *            prime < n^2 and its successor
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;          /* offset not defined below 2 */

    idx_t limit = n * n;                 /* n^2 */
    idx_t p = limit - 1;
    while (!is_prime(p)) --p;            /* largest prime < n^2 */

    idx_t q = p + 1;
    while (!is_prime(q)) ++q;            /* next prime after that */

    idx_t gap = q - p;
    seq_t result = (seq_t)2 * (seq_t)n;  /* 2*n as seq_t */
    result -= (seq_t)gap;                /* 2*n - gap */
    return result;
}
