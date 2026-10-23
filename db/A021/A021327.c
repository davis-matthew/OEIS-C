/*
 * A021327  Decimal expansion of 1/323.
 * https://oeis.org/A021327
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

/* Method: Compute the nth decimal digit of 1/323 via long division. */
seq_t a(idx_t n)
{
    /* The decimal expansion of 1/323 starts with 0.003095975...  */
    /* We keep a remainder r and repeatedly multiply by 10,
       extracting one digit per iteration. */
    const seq_t divisor = 323;      /* denominator */
    seq_t r = 1;                    /* initial remainder (the numerator) */
    seq_t digit = 0;

    for (idx_t i = 0; i <= n; ++i) {
        r *= 10;                    /* bring down the next decimal place */
        digit = r / divisor;        /* the current digit */
        r = r % divisor;            /* new remainder for next iteration */
    }
    return digit;
}
