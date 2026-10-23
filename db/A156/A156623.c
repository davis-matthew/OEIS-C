/*
 * A156623  Values of register b when register a becomes 0 for the two
 *          register machine {i[1], i[1], i[1], d[2,1], d[1,6], i[2], d[1,5],
 *          d[2,3]}.
 * https://oeis.org/A156623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = (3*a(n‑1) + 1) / 2 (for n > 1)
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Equivalently:  a(n) = a(n‑1) + (a(n‑1) + 1) / 2  */
/*  This generates A156623, identical to A061419. */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* not used, but safe */
    seq_t v = 1;                    /* a(1) = 1 */
    for (idx_t i = 2; i <= n; ++i) {
        v += (v + 1) / 2;           /* v = v + floor((v + 1)/2) */
    }
    return v;
}
