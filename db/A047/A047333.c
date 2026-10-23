/*
 * A047333  Duplicate of A032796.
 * https://oeis.org/A047333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 7*⌊(n-1)/5⌋ + s[(n-1) mod 5] where s = {1,2,3,5,6}.
 *            (i.e., a(n) is the n-th positive integer that is not congruent
 *            to 0 or 4 modulo 7.)
 * Range:     i8 n <= 25, i16 n <= 25, i32 n <= 25, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* map of remainders modulo 5 to the offset within each 7‑block */
    static const seq_t step[5] = {1, 2, 3, 5, 6};

    /* n ≥ 1 guaranteed by the caller (offset 1) */
    idx_t m = n - 1;              /* zero‑based index */
    seq_t block = (seq_t)(m / 5); /* how many full blocks of 5 kept numbers */
    int    r = (int)(m % 5);      /* position within the current block */

    return 7 * block + step[r];
}
