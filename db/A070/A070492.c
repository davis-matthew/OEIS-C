/*
 * A070492  a(n) = n^3 mod 30.
 * https://oeis.org/A070492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 30
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n modulo 30 using 64‑bit arithmetic to avoid overflow
       even when seq_t is as small as uint8_t.  The result of n % 30
       is guaranteed to be in [0,29], so it fits in any seq_t type.  */
    unsigned long long r = n % 30ULL;        /* r in [0,29]             */
    r = (r * r * r) % 30ULL;                /* (n^3) mod 30             */
    return (seq_t)r;                        /* cast to the desired type */
}
