/*
 * A021979  Decimal expansion of 1/975.
 * https://oeis.org/A021979
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

/* The sequence is the decimal expansion of 1/975.
   We compute the n-th digit by repeated long division:
   start with remainder 1, multiply by 10, take the quotient as the digit,
   and keep the remainder. */
seq_t a(idx_t n)
{
    seq_t r = 1;
    seq_t digit = 0;
    const seq_t DEN = 975;
    for (idx_t i = 0; i <= n; ++i) {
        r *= 10;
        digit = r / DEN;
        r = r % DEN;
    }
    return digit;
}
