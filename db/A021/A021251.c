/*
 * A021251  Decimal expansion of 1/247.
 * https://oeis.org/A021251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal digit n of 1/247 (a(n) is the n‑th decimal place, 0‑based). */
seq_t a(idx_t n)
{
    /* current remainder of 1/247; always < 247 */
    idx_t r = 1;
    seq_t digit = 0;

    /* To get the n‑th decimal digit we need n+1 iterations
       (the first iteration produces digit 0, the second digit 1, …). */
    for (idx_t i = 0; i <= n; ++i)
    {
        r *= 10U;          /* shift remainder left by one decimal place */
        digit = r / 247U;  /* integer part = next decimal digit */
        r = r % 247U;      /* new remainder for next iteration */
    }

    return digit;
}
