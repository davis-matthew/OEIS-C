/*
 * A069813  Maximum number of triangles in polyiamond with perimeter n.
 * https://oeis.org/A069813
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(n^2/6) - (0 if n mod 6 == 0 else 1) for n >= 3
 * Range:     u8 n <= 15, u16 n <= 429, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)          /* behaviour for n < 3 is irrelevant */
        return 0;
    /* compute n^2 as seq_t */
    seq_t sq   = (seq_t)n * ((seq_t)n);
    /* round towards nearest integer: (n^2 + 3)/6 */
    seq_t rnd  = (sq + 3) / 6;
    /* subtract 1 if n is not divisible by 6 */
    return rnd - ((n % 6) == 0 ? 0 : 1);
}
