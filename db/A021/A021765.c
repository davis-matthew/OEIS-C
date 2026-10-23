/*
 * A021765  Decimal expansion of 1/761.
 * https://oeis.org/A021765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = ⌊(10 * (10^n mod 761)) / 761⌋ The nth digit of the
 *            decimal expansion of 1/761 (offset 0). Here we compute 10^n
 *            modulo 761 by fast modular exponentiation and then extract the
 *            digit
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 10^n mod 761 */
    uint64_t r = 1;              /* current result (res = 10^k mod 761)   */
    uint64_t p = 10 % 761;       /* current power of 10 (10^(2^i) mod 761) */
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1u)             /* if lowest bit of exp is set  */
            r = (r * p) % 761;
        p = (p * p) % 761;        /* square the base   */
        exp >>= 1u;
    }
    /* digit is floor((r * 10) / 761) */
    return (seq_t)((r * 10) / 761);
}
