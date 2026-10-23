/*
 * A135133  a(n) = floor(S2(n)/3) mod 2, where S2(n) denotes the binary weight
 *          of n.
 * https://oeis.org/A135133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(S2(n)/3) mod 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  S2(n) is the binary weight (popcount) of n */
seq_t a(idx_t n)
{
    unsigned int pc = 0;
    idx_t t = n;
    while (t) {
        t &= t - 1;  /* clear the lowest set bit */
        pc++;
    }
    return (seq_t)((pc / 3) & 1);
}
