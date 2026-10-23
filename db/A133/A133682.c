/*
 * A133682  Number of regular complex polytopes in n-dimensional unitary
 *          complex space.
 * https://oeis.org/A133682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A133682: Number of regular complex polytopes in n-dimensional unitary complex space.
    The classification of regular complex polytopes yields only a few cases:
        n = 1 : 1
        n = 2 : 22
        n = 3 : 8
        n = 4 : 7
    For all dimensions n ≥ 5 there are exactly 3 such polytopes:
        the generalized n‑cube, its dual (generalized n‑cross‑polytope),
        and one further family counted as a single type.
    The function simply returns the appropriate value for the given n. */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 22;
    if (n == 3) return 8;
    if (n == 4) return 7;
    if (n >= 5) return 3;
    return 0;  /* unreachable for n ≥ 1 */
}
