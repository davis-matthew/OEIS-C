/*
 * A183898  Number of nondecreasing arrangements of n+3 numbers in 0..3 with
 *          each number being the sum mod 4 of three others.
 * https://oeis.org/A183898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n³ + 15 n² + 50 n – 84) / 6 for n > 1; a(1) = 5. The
 *            offset in OEIS is 1, so a(1) is the first term
 * Range:     u8 n <= 3, u16 n <= 35, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; behavior for n < 1 is irrelevant */
        return 0;

    if (n == 1)
        return (seq_t)5;

    /* use integer arithmetic only; all intermediate values fit in seq_t for the
       tested range (e.g., n ≤ 10 000 where the result is < 2⁶³). */
    seq_t nn = (seq_t)n;
    seq_t val = nn * nn * nn;                  /* n³ */
    val += ((seq_t)15) * nn * nn;              /* + 15 n² */
    val += ((seq_t)50) * nn;                   /* + 50 n */
    val -= (seq_t)84;                          /* − 84 */

    return val / (seq_t)6;
}
