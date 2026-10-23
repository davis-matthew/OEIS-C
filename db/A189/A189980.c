/*
 * A189980  a(n) is the number of incongruent two-color bracelets of n beads,
 *          10 from them are black (A005515), having a diameter of symmetry.
 * https://oeis.org/A189980
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(floor(n/2), 5)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 444, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 10. For smaller indices
       we return 0; this value is unused by OEIS tests. */
    if (n < 10)
        return 0;

    /* floor(n/2) */
    idx_t h = n >> 1;               /* integer division by 2 */

    /* For n ≥ 10, h ≥ 5, so the binomial coefficient is well‑defined. */
    seq_t r = 1;
    r = r * (seq_t)h;
    r = r * (seq_t)(h - 1);
    r = r * (seq_t)(h - 2);
    r = r * (seq_t)(h - 3);
    r = r * (seq_t)(h - 4);

    /* Divide by 5! = 120.  The division is exact. */
    return r / (seq_t)120;
}
