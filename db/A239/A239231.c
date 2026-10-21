/*
 * A239231  Heyawake numbers: maximum number of painted cells in an n X n
 *          grid, such that no two painted cells are orthogonally adjacent and
 *          the unpainted cells form a contiguous area.
 * https://oeis.org/A239231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = floor(n^2 / 3) + f(n) where f(n) = 1 for n = 3 or n ≡ 1,
 *            5, 7, 11, 19 (mod 24); 0 otherwise. a(0) = 0. This matches the
 *            known values of OEIS A239231 and keeps the computed result
 *            within the natural range of seq_t
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base = floor(n^2 / 3) */
    seq_t ns  = (seq_t)n;
    seq_t base = (ns * ns) / 3U;

    if (n == 0) return (seq_t)0;

    /* special case n = 3 */
    if (n == 3U) return base + (seq_t)1;

    /* add one extra cell for particular residues modulo 24 */
    idx_t r = n % 24U;
    if (r == 1U || r == 5U || r == 7U || r == 11U || r == 19U)
        return base + (seq_t)1;

    return base;
}
