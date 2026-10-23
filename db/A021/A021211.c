/*
 * A021211  Decimal expansion of 1/207.
 * https://oeis.org/A021211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^{n+1}/207) mod 10 Equivalent to performing
 *            decimal long‑division of 1/207 and taking the (n+1)th digit
 *            after the decimal point
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The remainder after dividing 1 by 207 starts as 1. */
    seq_t r = 1;
    seq_t digit = 0;

    /* Iterate n+1 times to reach the desired digit. */
    for (idx_t i = 0; i <= n; ++i) {
        /* Compute next digit: floor(r*10 / 207). */
        digit = (r * 10) / 207;
        /* Update remainder: r*10 mod 207. */
        r = (r * 10) % 207;
    }
    return digit;
}
