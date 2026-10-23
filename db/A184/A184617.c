/*
 * A184617  With nonadjacent forms: A184615(n) + A184616(n).
 * https://oeis.org/A184617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (n XOR (3*n)) / 2
 * Range:     u8 n <= 127, u16 n <= 8602, u32 n <= 8602, u64 n <= 8602,
 *            u128 n <= 8602, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS A184617: a(n) = (n ^ (n*3)) >> 1 */

seq_t a(idx_t n)
{
    /* Convert idx_t to seq_t and compute in that type to avoid overflow
       on 64‑bit seq_t when n is large. For arbitrary‑precision seq_t
       this handles any size. */
    seq_t sn = (seq_t)n;
    seq_t t  = sn * 3;
    return (sn ^ t) >> 1;
}
