/*
 * A021753  Decimal expansion of 1/749.
 * https://oeis.org/A021753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the (n+1)th decimal digit of 1/749 (0‑indexed)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = 749;   /* divisor of the decimal expansion */
    seq_t r = 1;            /* current remainder after division by 749 */
    seq_t digit = 0;        /* digit produced at each step */

    /* Iterate n+1 times to obtain the digit at position n+1 after the decimal point */
    for (idx_t i = 0; i <= n; ++i) {
        r = r * 10;           /* bring down the next decimal digit */
        digit = r / MOD;      /* integer division gives the next digit (0‑9) */
        r = r % MOD;          /* update remainder for next iteration */
    }

    return digit;            /* return the nth digit (0‑based) */
}
