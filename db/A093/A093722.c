/*
 * A093722  Integers of the form (k^2 - 1) / 120.
 * https://oeis.org/A093722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (k^2 - 1)/120 where k > 0 satisfies k^2 ≡ 1 (mod 120).
 *            The sequence is given by a quasi‑polynomial over n-1: Let m =
 *            n-1, q = m / 4, r = m % 4. r = 0: a(m) = q * (15*q + 1) / 2 r =
 *            1: a(m) = (3*q + 1) * (5*q + 2) / 2 r = 2: a(m) = (3*q + 2) *
 *            (5*q + 3) / 2 r = 3: a(m) = (q + 1) * (15*q + 1...
 * Range:     u8 n <= 23, u16 n <= 374, u32 n <= 9550, u64 n <= 9550,
 *            u128 n <= 9550, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset: a(1) corresponds to m = 0 */
    idx_t m = n - 1;
    idx_t q = m >> 2;          /* q = m / 4 */
    idx_t r = m & 3;           /* r = m % 4 */
    seq_t qs = (seq_t)q;       /* cast once for seq_t operations */

    switch (r) {
        case 0: /* a(4*q) */
            return qs * ((seq_t)15 * qs + 1) / 2;
        case 1: /* a(4*q + 1) */
            return ((seq_t)3 * qs + 1) * ((seq_t)5 * qs + 2) / 2;
        case 2: /* a(4*q + 2) */
            return ((seq_t)3 * qs + 2) * ((seq_t)5 * qs + 3) / 2;
        default: /* r == 3, a(4*q + 3) */
            return (qs + 1) * ((seq_t)15 * qs + 14) / 2;
    }
}
