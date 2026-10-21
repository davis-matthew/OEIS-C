/*
 * A098427  Order of symmetry group of the five Platonic solids sorted on the
 *          number of faces (A053016).
 * https://oeis.org/A098427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(1)=24, a(2)=48, a(3)=48, a(4)=120, a(5)=120, a(n)=0 for n>5
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Constants 24,48,120 fit within any seq_t type */
    if (n == 1)
        return (seq_t)24;
    if (n == 2 || n == 3)
        return (seq_t)48;
    if (n == 4 || n == 5)
        return (seq_t)120;
    return (seq_t)0;
}
