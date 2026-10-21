/*
 * A081823  Decimal expansion of the elementary charge e in coulombs (C).
 * https://oeis.org/A081823
 *
 * Offset:    -18    (a(-18) is the first term)
 * Keywords:  cons, nonn, full, fini
 * Formula:   not stated
 * Range:     u8 n <= -9, u16 n <= -9, u32 n <= -9, u64 n <= -9, u128 n <= -9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A081823: digits of 1.602176634 × 10⁻¹⁹.  */
seq_t a(idx_t n)
{
    /* The significant digits of 1.602176634 are stored left‑to‑right. */
    static const seq_t digits[10] = { 1, 6, 0, 2, 1, 7, 6, 6, 3, 4 };

    /* The sequence is defined for n ≥ -18.  For n < -18 we simply return 0. */
    if (n < -18)
        return 0;

    /* Map the index n to an array position.  For n = -18 … -9 we get 0 … 9. */
    int idx = (int)(n + 18);
    if (idx >= 0 && idx < 10)
        return digits[idx];

    /* For n > -9 all trailing digits are 0 (finite expansion). */
    return 0;
}
