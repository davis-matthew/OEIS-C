/*
 * A033433  a(n) = floor(10000/sqrt(n)).
 * https://oeis.org/A033433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(10000 / sqrt(n)) (OEIS A033433)
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalently, a(n) = floor( sqrt( 100000000 / n ) )  for 1 ≤ n ≤ 10^8, 0 otherwise. */

#include <stdint.h>

/* Integer square‑root for uint64_t, using the bit‑by‑bit algorithm
   (Hacker's Delight, section 10-4).  The result fits in uint64_t. */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t res = 0;
    uint64_t bit = 1ULL << 62;          /* 1 << (2*31), highest power of 4 <= UINT64_MAX */

    while (bit > x) bit >>= 2;          /* find the highest power of four <= x */

    while (bit != 0) {
        if (x >= res + bit) {
            x -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

/* Main sequence function.  idx_t is typically uint64_t. */
seq_t a(idx_t n)
{
    /* For n outside the defined range, the OEIS entry says a(n) = 0 */
    if (n == 0 || n > 100000000ULL) return 0;

    /* Compute 100000000 / n (exact integer division).  The result fits in 32 bits. */
    uint64_t q = 100000000ULL / n;

    /* a(n) = floor( sqrt( q ) ).  Use the integer sqrt helper. */
    return (seq_t)isqrt_u64(q);
}
