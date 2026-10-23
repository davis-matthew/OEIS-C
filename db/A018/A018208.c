/*
 * A018208  Expansion of 1/((1-3x)(1-11x)(1-12x)).
 * https://oeis.org/A018208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 26*a(n-1) - 201*a(n-2) + 396*a(n-3) with initial values
 *            a(0)=1, a(1)=26, a(2)=475
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)26;
    if (n == 2) return (seq_t)475;

    /* Keep only the last three terms to save memory. */
    seq_t v0 = 1;   /* a(n-3) */
    seq_t v1 = 26;  /* a(n-2) */
    seq_t v2 = 475; /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = v2 * 26;
        next -= v1 * 201;
        next += v0 * 396;

        v0 = v1;
        v1 = v2;
        v2 = next;
    }
    return v2;
}
