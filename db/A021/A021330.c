/*
 * A021330  Decimal expansion of 1/326.
 * https://oeis.org/A021330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th decimal digit (0‑based) of the fraction 1/326.
 * The sequence a(n) is the digit after the n‑th decimal place of 1/326,
 * so a(0)=0, a(1)=0, a(2)=3, … */
seq_t a(idx_t n)
{
    static const seq_t DEN = (seq_t)326;
    seq_t r = (seq_t)1;          /* start with remainder = numerator          */
    seq_t digit = 0;

    for (idx_t i = 0; i <= n; ++i) {
        r *= 10;
        digit = r / DEN;
        r = r % DEN;
    }
    return digit;
}
