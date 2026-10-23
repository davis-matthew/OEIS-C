/*
 * A047670  Row 6 of array in A047666.
 * https://oeis.org/A047670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^6 + 50*n^4 + 173*n^2 + 45)/45
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 67, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Guard against n == 0, although not needed for the offset */
    if (n == 0) return (seq_t)0;

    /* Use 128‑bit arithmetic for intermediate results.  This
       guarantees correct evaluation for all n that fit in seq_t
       and avoids overflow when seq_t is 64‑bit. */
    __uint128_t nn = n;          /* n as 128‑bit */
    __uint128_t n2 = nn * nn;    /* n^2 */
    __uint128_t n4 = n2 * n2;    /* n^4 */
    __uint128_t n6 = n4 * n2;    /* n^6 */

    __uint128_t value = 2 * n6 + 50 * n4 + 173 * n2 + 45;
    __uint128_t result128 = value / 45;  /* exact division */

    return (seq_t)result128;
}
