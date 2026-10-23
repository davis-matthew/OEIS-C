/*
 * A137302  Decimal expansion of 99999/9801.
 * https://oeis.org/A137302
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the (n‑2)th digit of the decimal representation of
 *            99999/9801, where index 2 is the most significant digit of the
 *            integer part
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 100, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Immediate answers for the integer part of the fraction (10). */
    if (n == 2) return 1;   /* first digit of 10 */
    if (n == 3) return 0;   /* second digit of 10 */

    /* Remaining digits come from the continued fraction expansion. */
    const seq_t NUM  = (seq_t)99999;
    const seq_t DEN  = (seq_t)9801;

    /* Compute the remainder after extracting the integer part (10). */
    seq_t rem = NUM - ((NUM / DEN) * DEN);  /* rem = 1989 */

    /* Perform long division: generate digits until we reach the requested one. */
    seq_t digit = 0;
    /* Number of decimal digits to skip before the desired one. */
    idx_t steps = n - 4; /* n=4 gives the first decimal digit (2) */
    for (idx_t i = 0; i <= steps; ++i) {
        rem  = rem * 10;
        digit = rem / DEN;
        rem   = rem % DEN;
    }
    return digit;
}
