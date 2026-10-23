/*
 * A134804  Remainder of triangular number A000217(n) modulo 9.
 * https://oeis.org/A134804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n*(n+1)/2) mod 9
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n and n+1 are unsigned 64‑bit, so wraparound is defined.
       Work modulo 9 to avoid overflow irrespective of the size of seq_t. */
    uint64_t mod = 9ULL;                /* modulo value */
    uint64_t n9   = n % mod;            /* n mod 9   */
    uint64_t n1_9 = (n + 1) % mod;      /* (n+1) mod 9 */

    /* product modulo 9 */
    uint64_t prod = (n9 * n1_9) % mod;

    /* divide by 2 modulo 9 – inverse of 2 mod 9 is 5 since 2*5=10≡1 (mod 9) */
    uint64_t res = (prod * 5ULL) % mod;

    return (seq_t)res;
}
