/*
 * A003886  Degrees of irreducible representations of group L2(19).
 * https://oeis.org/A003886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   degrees of the irreducible characters of PSL(2,q), q = 19 (q ==
 *            3 mod 4), in increasing order: 1, (q-1)/2 twice, q-1 repeated
 *            (q-3)/4 times, q once, q+1 repeated (q-3)/4 times. The finite
 *            sequence has (q+5)/2 = 12 terms; return 0 for n beyond that
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const idx_t q = 19;
    const idx_t r = (q - 3) / 4;          /* multiplicity of q-1 and of q+1 */
    idx_t k;
    if (n < 1) return 0;
    k = n - 1;                             /* 0-based position */
    if (k < 1) return (seq_t)1;
    k -= 1;
    if (k < 2) return (seq_t)((q - 1) / 2);
    k -= 2;
    if (k < r) return (seq_t)(q - 1);
    k -= r;
    if (k < 1) return (seq_t)q;
    k -= 1;
    if (k < r) return (seq_t)(q + 1);
    return (seq_t)0;
}
