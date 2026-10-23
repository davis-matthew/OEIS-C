/*
 * A196368  Characteristic function of numbers having in decimal
 *          representation only distinct adjacent digits.
 * https://oeis.org/A196368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if the decimal representation of n has no * adjacent
 *            equal digits; otherwise a(n) = 0. * * This follows the
 *            definition of OEIS sequence A196368
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage required for the testing harness */
seq_t a(idx_t n)
{
    /* Single-digit numbers always satisfy the property */
    if (n < 10)
        return (seq_t)1;

    /* Examine digits from least significant to most significant. */
    idx_t prev = n % 10;   /* last digit */
    n /= 10;               /* remove last digit */

    while (n > 0)
    {
        idx_t cur = n % 10;    /* next digit to the left */
        if (cur == prev)
            return (seq_t)0;   /* adjacent equal digits found */
        prev = cur;
        n /= 10;               /* continue with remaining digits */
    }
    return (seq_t)1;   /* all adjacent pairs are distinct */
}
