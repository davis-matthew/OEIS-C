/*
 * A070493  a(n) = n^3 mod 31.
 * https://oeis.org/A070493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 31
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 31 to keep intermediates small. */
    idx_t r = n % 31u;

    /* Compute r^3 modulo 31 using 64‑bit arithmetic. */
    idx_t t = (r * r) % 31u;
    t = (t * r) % 31u;

    return (seq_t)t;
}
