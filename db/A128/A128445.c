/*
 * A128445  Number of facets of the Alternating Sign Matrix polytope ASM(n).
 * https://oeis.org/A128445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4 * ((n-2)^2 + 1) for n ≥ 3; a(0) = 1, a(1) = 1, a(2) = 2
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2) {
        switch (n) {
            case 0:
            case 1:
                return 1;
            case 2:
                return 2;
        }
    }
    seq_t nm2 = (seq_t)n - 2;
    return ((seq_t)4) * (nm2 * nm2 + 1);
}
