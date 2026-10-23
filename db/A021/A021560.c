/*
 * A021560  Decimal expansion of 1/556.
 * https://oeis.org/A021560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ⌊(10 * (10^n mod 556)) / 556⌋ (nth decimal digit of
 *            1/556)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned int rem = 1;          /* current remainder (after n digits) */
    for (idx_t i = 0; i < n; ++i)  /* perform n steps of long division */
        rem = (rem * 10u) % 556u;
    unsigned int digit = (rem * 10u) / 556u;   /* extract next digit */
    return (seq_t)digit;
}
