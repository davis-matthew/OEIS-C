/*
 * A070447  a(n) = n^2 mod 25.
 * https://oeis.org/A070447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 25
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 25 first to avoid overflow. */
    seq_t r = (seq_t)(n % 25);
    /* Compute r*r using a wider integer type. */
    uint64_t prod = (uint64_t)r * (uint64_t)r;
    return (seq_t)(prod % 25);
}
