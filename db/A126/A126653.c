/*
 * A126653  A 3 X 3 magic square with magic sum 45: the Loh-Shu square A033812
 *          multiplied by 3.
 * https://oeis.org/A126653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) gives the entries of the 3×3 Lo Shu magic square (multiplied by 3),
 * read from bottom to top, left to right.  The sequence is periodic with
 * period 9: 24,3,18,9,15,21,12,27,6. */
seq_t a(idx_t n)
{
    /* base values of the Lo Shu square */
    static const seq_t base[9] = {4, 9, 2,
                                 3, 5, 7,
                                 8, 1, 6};

    /* index within one period */
    idx_t t = (n - 1) % 9;

    /* compute row (from bottom) and column (from left) */
    idx_t r = 2 - (t / 3);
    idx_t c = t % 3;

    /* linear index in base array */
    idx_t idx = r * 3 + c;

    /* value is 3 times the base entry */
    return 3 * base[idx];
}
