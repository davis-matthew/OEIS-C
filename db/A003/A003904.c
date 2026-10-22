/*
 * A003904  Degrees of irreducible representations of Janko group J1.
 * https://oeis.org/A003904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   Degrees of irreducible representations of Janko group J1 (finite
 *            sequence, 15 terms)
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t degrees[] = {
        1, 56, 56, 76, 76, 77, 77, 77, 120, 120, 120, 133, 133, 133, 209
    };
    
    if (n < 1 || n > 15)
        return 0;
    
    return degrees[n - 1];
}
