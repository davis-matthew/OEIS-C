/*
 * A003887  Degrees of irreducible representations of group L2(23).
 * https://oeis.org/A003887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   degrees of the irreducible characters of PSL(2,q), q = 23 (q ==
 *            3 mod 4), sorted: 1; (q-1)/2 twice; q-1 repeated (q-3)/4 times;
 *            q; q+1 repeated (q-3)/4 times. Finite sequence: (q+5)/2 = 14
 *            terms
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const idx_t q = 23;
    const idx_t m = (q - 3) / 4;   /* multiplicity of q-1 and of q+1 */
    idx_t pos;

    if (n < 1) return (seq_t)0;
    pos = 1;
    if (n == pos) return (seq_t)1;
    pos += 1;
    if (n < pos + 2) return (seq_t)((q - 1) / 2);
    pos += 2;
    if (n < pos + m) return (seq_t)(q - 1);
    pos += m;
    if (n == pos) return (seq_t)q;
    pos += 1;
    if (n < pos + m) return (seq_t)(q + 1);
    return (seq_t)0;
}
