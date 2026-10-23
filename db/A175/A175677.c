/*
 * A175677  Binomial(n,4) mod n.
 * https://oeis.org/A175677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,4) mod n (computed directly without relying on
 *            binomial helper)
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 4) return 0;
    uint64_t n64 = (uint64_t)n;
    uint64_t prod = n64 * (n64 - 1) * (n64 - 2) * (n64 - 3);
    prod /= 24;                /* exact division since binomial coefficient is integer */
    uint64_t res = prod % n64; /* modulo n */
    return (seq_t)res;
}
