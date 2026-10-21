/*
 * A082390  Numbers on a computer numpad, read in a clockwise spiral.
 * https://oeis.org/A082390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dumb, easy, fini, full, nonn
 * Formula:   a(n) is the n‑th element of the repeating permutation * of the
 *            digits {0,1,2,3,4,5,6,7,8,9} read on a computer numpad * in a
 *            clockwise spiral starting at the key 0. * The pattern of one
 *            cycle (n = 1..10) is: * 0,1,4,7,8,9,6,3,2,5 * thereafter it
 *            repeats with period 10
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* 10‑element cycle of the numpad digits in spiral order */
    static const seq_t cycle[10] = {0, 1, 4, 7, 8, 9, 6, 3, 2, 5};
    /* Zero‑based index within the cycle */
    idx_t idx = (n - 1) % 10;
    return cycle[idx];
}
