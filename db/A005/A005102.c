/*
 * A005102  Minimal determinant of any n-dimensional norm 2 lattice.
 * https://oeis.org/A005102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Minimal determinant of any n-dimensional norm 2 lattice. For n
 *            >= 15, a(n) = 1 (established via lattice theory). For n < 15,
 *            computed from known optimal lattices in each dimension
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n >= 15) return 1;
    
    static const seq_t table[] = {
        1, 2, 3, 4, 4, 4, 3, 2, 1, 2, 3, 2, 1, 2, 1
    };
    
    return table[n];
}
