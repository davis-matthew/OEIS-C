/*
 * A057061  a(n) = number of the column of (R(i,j)) that contains prime(n),
 *          where R(i,j) is the rectangle with antidiagonals 1; 2,3; 4,5,6;
 *          ...
 * https://oeis.org/A057061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A004736(prime(n))
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Retrieve the n-th prime (prime(1) = 2, prime(2) = 3, ...) */
    uint64_t prime = get_prime(n);          /* n >= 1 */

    /* Find the diagonal d such that d(d+1)/2 >= prime */
    uint64_t d = 1;
    while ( (d * (d + 1)) >> 1 < prime )
        ++d;

    /* Total numbers before diagonal d: d(d‑1)/2 */
    uint64_t before = (d * (d - 1)) >> 1;
    uint64_t pos    = prime - before;       /* 1‑based position within diagonal */

    /* Column index: j = d - pos + 1 */
    uint64_t col = d - pos + 1;

    return (seq_t)col;
}
