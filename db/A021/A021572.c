/*
 * A021572  Decimal expansion of 1/568.
 * https://oeis.org/A021572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth decimal digit of 1/568 via successive remainder multiplication */
seq_t a(idx_t n)
{
    seq_t rem = 1;          /* initial remainder: 1 (since 1/568) */
    seq_t digit = 0;        /* digit at the current step */
    const idx_t mod = 568;  /* denominator */

    for (idx_t i = 0; i <= n; ++i)
    {
        idx_t tmp = (idx_t)rem * 10;         /* 10 * remainder (wider type to avoid overflow) */
        digit = (seq_t)(tmp / mod);          /* current decimal digit */
        rem   = (seq_t)(tmp % mod);          /* updated remainder for next digit */
    }
    return digit;                            /* a(n) */
}
