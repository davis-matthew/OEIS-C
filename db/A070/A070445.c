/*
 * A070445  a(n) = n^2 mod 23.
 * https://oeis.org/A070445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 23
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 23 first; this keeps the intermediate
       values small enough for all builds, including the 8‑bit
       sequence type. */
    unsigned int r = (unsigned int)(n % 23);
    unsigned int prod = r * r;        /* 0 <= prod <= 22*22 = 484 */
    unsigned int mod = prod % 23;      /* 0 <= mod < 23 */
    return (seq_t)mod;
}
