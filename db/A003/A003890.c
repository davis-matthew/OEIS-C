/*
 * A003890  Degrees of irreducible representations of group L2(29).
 * https://oeis.org/A003890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   character degrees of PSL(2,q), q = 29 (q == 1 mod 4), sorted: 1;
 *            (q+1)/2 twice; q-1 with multiplicity (q-1)/4; q once; q+1 with
 *            multiplicity (q-5)/4. Total (q+5)/2 = 17 terms
 * Range:     u8 n <= 17, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const idx_t q = 29;
    idx_t k = n;
    if (k < 1) return 0;
    if (k == 1) return (seq_t)1;
    k -= 1;
    if (k <= 2) return ((seq_t)q + 1) / 2;
    k -= 2;
    if (k <= (q - 1) / 4) return (seq_t)q - 1;
    k -= (q - 1) / 4;
    if (k == 1) return (seq_t)q;
    k -= 1;
    if (k <= (q - 5) / 4) return (seq_t)q + 1;
    return 0;
}
