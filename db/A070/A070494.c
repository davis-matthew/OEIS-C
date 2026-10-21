/*
 * A070494  a(n) = n^3 mod 32.
 * https://oeis.org/A070494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 32
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 32 first to avoid overflow */
    unsigned r = (unsigned)(n & 31);          /* r = n % 32 */
    unsigned prod = r * r * r;                /* r^3 fits in 32 bits */
    return (seq_t)(prod & 31);                /* prod % 32 */
}
