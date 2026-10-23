/*
 * A021755  Decimal expansion of 1/751.
 * https://oeis.org/A021755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1) / 751) mod 10 (decimal digit of 1/751)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* We compute 10^(n+1) modulo 7510.  Since 751*10 = 7510,
       the quotient of that value divided by 751 gives exactly
       the desired decimal digit. */
    unsigned int mod = 1;            /* 10^0 mod 7510 */
    idx_t i;

    /* Perform n+1 modular multiplications by 10 */
    for (i = 0; i <= n; ++i) {
        mod = (mod * 10) % 7510;
    }

    /* The digit is floor(mod / 751), which lies between 0 and 9 */
    return (seq_t)(mod / 751);
}
