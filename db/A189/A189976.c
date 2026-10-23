/*
 * A189976  a(n) is the number of incongruent two-color bracelets of n beads,
 *          8 of them black (A005514), having a diameter of symmetry.
 * https://oeis.org/A189976
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(floor(n/2), 4)
 * Range:     u8 n <= 21, u16 n <= 73, u32 n <= 848, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Floor division of n by 2 is obtained by right‑shifting n. */

seq_t a(idx_t n)
{
    /* floor(n/2) */
    idx_t m = n >> 1;

    /* for n < 8 (i.e., m < 4) the value is 0 – the definition starts at n = 8 */
    if (m < 4)
        return 0;

    /* Compute C(m,4) multiplicatively to avoid overflow of factorials.
       res = (m-3)*(m-2)*(m-1)*m / 4!  */
    seq_t res = 1;
    for (idx_t i = 1; i <= 4; ++i) {
        /* numerator terms: (m-4+i) for i=1..4 -> m-3 .. m */
        res = res * (seq_t)(m - 4 + i) / (seq_t)i;
    }

    return res;
}
