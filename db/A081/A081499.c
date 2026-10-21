/*
 * A081499  Sum at 45 degrees to horizontal in triangle of A081498.
 * https://oeis.org/A081499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ((n+⌊n/2⌋+1)(n-⌊n/2⌋)/2) - ((⌈n/2⌉+1)⌈n/2⌉(⌈n/2⌉-1)/6)
 * Range:     i8 n <= 26, i16 n <= 122, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 is assumed, as per OEIS offset 1 */
    seq_t nf = (seq_t)n;          /* convert to seq_t for arithmetic      */
    seq_t floor_half = nf / 2;    /* ⌊n/2⌋ (integer division on unsigned) */
    seq_t r = (nf + 1) / 2;       /* ceil(n/2)                            */

    /* first part: (n + floor(n/2) + 1) * (n - floor(n/2)) / 2  */
    seq_t part1 = (nf + floor_half + 1) * (nf - floor_half) / 2;

    /* second part: (r + 1) * r * (r - 1) / 6  */
    seq_t part2 = (r + 1) * r * (r - 1) / 6;

    return part1 - part2;
}
