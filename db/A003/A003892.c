/*
 * A003892  Degrees of irreducible representations of group L2(32).
 * https://oeis.org/A003892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   for PSL(2,q) with q = 32 (q even), the irreducible degrees in
 *            order are 1 (once), q-1 (q/2 times), q (once), q+1 ((q-2)/2
 *            times), q+1 = 33 terms in all. So a(1) = 1; a(n) = q-1 for 2 <=
 *            n <= q/2+1; a(q/2+2) = q; a(n) = q+1 for q/2+3 <= n <= q+1
 * Range:     u8 n <= 33, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const idx_t q = 32;
    if (n <= 1) return (seq_t)1;
    if (n <= q / 2 + 1) return (seq_t)q - 1;
    if (n == q / 2 + 2) return (seq_t)q;
    if (n <= q + 1) return (seq_t)q + 1;
    return (seq_t)0;
}
