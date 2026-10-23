/*
 * A120928  Number of "ups" and "downs" in the permutations of [n] if either a
 *          previous counted "up" ("down") or a "void" precedes an "up"
 *          ("down") which then will be counted also.
 * https://oeis.org/A120928
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * (3*n - 1) / 6 for n > 2, a(2) = 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;           /* indices below the offset are ignored */
    if (n == 2)
        return (seq_t)2;           /* special case as given in OEIS */

    seq_t result = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;        /* factorial part */

    result *= ((seq_t)3 * (seq_t)n - (seq_t)1); /* multiply by (3*n - 1) */
    result /= (seq_t)6;                          /* divide by 6 */

    return result;
}
