/*
 * A060817  Size of the automorphism group of the alternating group A_n.
 * https://oeis.org/A060817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! for n ≠ 2,3,6; a(1) = 1, a(2) = 1, a(3) = 2, a(6) =
 *            2·n!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 1) return 0;              /* undefined for n < 1 */

    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    if (n == 2) return 1;             /* Aut(A₂) ≅ S₁, size 1 */
    if (n == 3) return 2;             /* Aut(A₃) ≅ C₂, size 2 */
    if (n == 6) return fact * 2;      /* Aut(A₆) ≅ S₆ ⋊ C₂, size 2·6! */

    return fact;                      /* otherwise |Aut(Aₙ)| = n! */
}
