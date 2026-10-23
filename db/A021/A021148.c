/*
 * A021148  Decimal expansion of 1/144.
 * https://oeis.org/A021148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit of the decimal expansion of 1/144 (computed by
 *            performing long division: start with remainder 1, repeatedly
 *            multiply by 10, the quotient is the next digit, the remainder is
 *            updated modulo 144. a(0)=0, a(1)=0, a(2)=6, …)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t divisor = 144;      /* 1/144 */
    seq_t remainder = 1;            /* initial numerator */
    seq_t digit = 0;

    /* perform the long‑division step n+1 times: step 0 gives a(0) */
    for (idx_t i = 0; i <= n; ++i) {
        remainder *= 10;            /* bring down next decimal digit */
        digit = remainder / divisor;
        remainder %= divisor;
    }

    return digit;
}
