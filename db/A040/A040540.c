/*
 * A040540  Continued fraction for sqrt(564).
 * https://oeis.org/A040540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less, mult
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A040540: continued‑fraction coefficients for sqrt(564)
    a(0) = 23
    for n ≥ 1, a(n) = {1, 2, 1, 46}[(n-1) mod 4]   (period 4) */
seq_t a(idx_t n) {
    if (n == 0) {
        return 23;
    }
    idx_t r = (n - 1) % 4;
    if (r == 0) return 1;
    if (r == 1) return 2;
    if (r == 2) return 1;
    return 46;          /* r == 3 */
}
