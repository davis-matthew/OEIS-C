/*
 * A047506  Numbers that are congruent to {4, 6, 7} mod 8.
 * https://oeis.org/A047506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Computes a(n) for OEIS A047506. The sequence consists of numbers
 *            congruent to 4, 6, or 7 (mod 8). Forbidden residues: ... are
 *            excluded. The following closed formulas hold (k ≥ 1): a(3k-2) =
 *            8k - 4 a(3k-1) = 8k - 2 a(3k) = 8k - 1 These are evaluated using
 *            idx_t intermediates to avoid overflow in t...
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the official offset – a(1) is the first term. */
    if (n == 0) {
        return 0;          /* undefined for the OEIS offset, harmless fallback */
    }

    idx_t r = n % 3;
    if (r == 0) {                            /* n = 3k     */
        idx_t k = n / 3;
        return (seq_t)(8 * k - 1);
    } else if (r == 1) {                     /* n = 3k-2   */
        idx_t k = (n + 2) / 3;
        return (seq_t)(8 * k - 4);
    } else {                                 /* r == 2: n = 3k-1 */
        idx_t k = (n + 1) / 3;
        return (seq_t)(8 * k - 2);
    }
}
