/*
 * A021326  Decimal expansion of 1/322.
 * https://oeis.org/A021326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 322) mod 10 (the nth digit after the
 *            decimal point in the decimal expansion of 1/322)
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The long‑division algorithm: start with
       remainder 1 and repeatedly multiply by 10,
       take the quotient as the next digit, and
       keep the updated remainder. */
    seq_t rem = 1;          /* current remainder */
    seq_t digit = 0;        /* current digit */

    for (idx_t i = 0; i <= n; ++i) {
        rem = rem * (seq_t)10;   /* bring down next zero */
        digit = rem / (seq_t)322;/* next decimal digit */
        rem = rem % (seq_t)322;  /* new remainder for next step */
    }
    return digit; /* 0 … 9 */
}
