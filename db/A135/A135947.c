/*
 * A135947  a(n)=(floor(3*S2(n)/2)) mod 2, where S2(n) is the binary weight of
 *          n.
 * https://oeis.org/A135947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (floor(3*S2(n)/2)) mod 2
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where S2(n) is the binary weight (popcount) of n */

seq_t a(idx_t n)
{
    /* Count set bits of n. */
    unsigned int pc = __builtin_popcountll((unsigned long long)n);

    /* Compute floor(3*pc/2) and take its parity. */
    unsigned int val = (3 * pc) / 2;

    return (seq_t)(val & 1);
}
