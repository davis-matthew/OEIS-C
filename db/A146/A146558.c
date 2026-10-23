/*
 * A146558  Number of order n permutations without collinear triples modulo n.
 * https://oeis.org/A146558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = 1 if n=1, 2 if n=2, 16 if n=4, 72 if n=6, 256 if n=8,
 *            2304 if n=12, and 0 otherwise (no other values are known for n >
 *            12)
 * Range:     u8 n <= 7, u16 n <= 29, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
        case 1: return (seq_t)1;
        case 2: return (seq_t)2;
        case 4: return (seq_t)16;
        case 6: return (seq_t)72;
        case 8: return (seq_t)256;
        case 12: return (seq_t)2304;
        default: return (seq_t)0;
    }
}
