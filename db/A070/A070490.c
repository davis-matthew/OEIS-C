/*
 * A070490  a(n) = n^3 mod 28.
 * https://oeis.org/A070490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 28
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 28 to keep intermediate values small. */
    uint64_t r  = n % 28ULL;          /* 0 ≤ r < 28 */
    uint64_t res = (r * r % 28ULL) * r % 28ULL; /* r^3 mod 28 */
    return (seq_t)res;               /* cast to arbitrary precision if needed */
}
