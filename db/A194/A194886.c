/*
 * A194886  Units' digits of the nonzero decagonal numbers.
 * https://oeis.org/A194886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (n * (4*n - 3)) mod 10 (OEIS A194886)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Reduce n modulo 10 to keep numbers small and avoid overflow. */
    uint64_t r = n % 10;                     /* r = n mod 10 */
    uint64_t s = (4 * r) % 10;                /* s = 4*n mod 10 */
    s = (s + 10 - 3) % 10;                   /* s = (4*n - 3) mod 10 */
    uint64_t res = (r * s) % 10;             /* a(n) mod 10 */

    return (seq_t)res;
}
