/*
 * A003884  Degrees of irreducible representations of group L2(16).
 * https://oeis.org/A003884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   degrees of irreducible characters of L2(q), q = 2^4 = 16 (q
 *            even), sorted: 1 (once), q-1 (q/2 times), q (once), q+1 ((q-2)/2
 *            times); a(n) = 0 for n > q+1
 * Range:     u8 n <= 17, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const idx_t q = (idx_t)1 << 4;     /* field size 2^4 */
    idx_t pos = n;                      /* 1-based position */
    if (pos == 0) return 0;
    if (pos == 1) return 1;
    pos -= 1;
    if (pos <= q / 2) return (seq_t)q - 1;      /* discrete-series characters */
    pos -= q / 2;
    if (pos == 1) return (seq_t)q;               /* Steinberg character */
    pos -= 1;
    if (pos <= (q - 2) / 2) return (seq_t)q + 1; /* principal-series characters */
    return 0;
}
