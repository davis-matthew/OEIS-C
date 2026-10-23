/*
 * A124758  Product of the parts of the compositions in standard order.
 * https://oeis.org/A124758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, look, tabf
 * Formula:   a(n) = product of parts of the composition in standard order
 * Range:     u8 n <= 9433, u16 n <= 9433, u32 n <= 9433, u64 n <= 9433,
 *            u128 n <= 9433, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The composition of n (n ≥ 1) is obtained by taking the positions of 1-bits in the binary
 * representation of n (counting from the least significant bit, starting at 1),
 * prepending 0, computing successive differences, and reversing the result.
 * The product of these differences equals the product of the parts,
 * which is returned. For n = 0 the composition is empty and a(0) = 1. */

seq_t a(idx_t n) {
    if (n == 0) return 1;                 /* empty composition */
    seq_t prod = 1;                        /* product accumulator */
    idx_t prev = 0;                        /* previous set-bit position */
    idx_t pos  = 1;                        /* current bit position (LSB = 1) */
    while (n > 0) {
        if (n & 1) {                       /* bit is set at position pos */
            seq_t diff = (seq_t)(pos - prev);
            prod *= diff;
            prev = pos;
        }
        n >>= 1;                           /* move to next bit */
        pos++;
    }
    return prod;
}
