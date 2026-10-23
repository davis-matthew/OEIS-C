/*
 * A137282  Final digit of A136408(n).
 * https://oeis.org/A137282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-8) for n >= 11. The first 11 terms are given below
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage required by the build system. */
seq_t a(idx_t n)
{
    /* These values are the initial segment of the sequence. */
    static const seq_t init[19] = {
        /* n = 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 */
        1, 2, 4, 7, 3, 7, 5, 7, 1, 7, 9, 7, 3, 7, 5, 7, 1, 7, 9
    };

    /* Handle the pre‑period directly. */
    if (n < 11)
        return init[(idx_t)n];

    /* For n >= 11 the sequence repeats every 8 terms. */
    idx_t k = (n - 11) & 7;             /* modulo 8 using a bit mask. */
    return init[11 + k];
}
