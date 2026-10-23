/*
 * A021383  Decimal expansion of 1/379.
 * https://oeis.org/A021383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ⌊10^(n+1)/379⌋ – 10·⌊10^n/379⌋ (the nth decimal digit of
 *            1/379, 0‑indexed)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 1;          /* current remainder (initially 1) */
    seq_t digit = 0;      /* last computed digit */

    for (idx_t i = 0; i <= n; ++i) {
        r *= 10;
        digit = r / (seq_t)379;   /* quotient gives the digit */
        r = r % (seq_t)379;       /* new remainder for next iteration */
    }
    return digit;          /* digit at index n */
}
