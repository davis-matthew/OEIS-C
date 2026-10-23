/*
 * A168046  Characteristic function of zerofree numbers in decimal
 *          representation.
 * https://oeis.org/A168046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 1 if n has no digit 0 in its decimal representation, else
 *            0. a(0) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) is the characteristic function of zerofree numbers. */
seq_t a(idx_t n)
{
    /* Special case: 0 contains a zero digit. */
    if (n == 0)
        return (seq_t)0;

    /* Scan decimal digits of n. */
    for (; n > 0; n /= 10U) {
        if ((n % 10U) == 0U)
            return (seq_t)0;   /* zero digit found */
    }
    return (seq_t)1;              /* all digits are non‑zero */
}
