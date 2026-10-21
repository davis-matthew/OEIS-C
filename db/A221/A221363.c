/*
 * A221363  Decimal expansion of the Pythagorean comma.
 * https://oeis.org/A221363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy, fini, full
 * Formula:   a(1) = 1 ; a(n) = the (n-1)-th decimal digit of 3^12 / 2^19 for
 *            n >= 2. For n > 20 the expansion terminates and the digit
 *            sequence continues with 0
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Integer part */
    if (n == 1)
        return 1;

    /* For n < 2 the offset is not used */
    if (n < 2)
        return 0;

    /* Parameters of the fraction 3^12 / 2^19 */
    static const seq_t NUM = (seq_t)531441U;   /* 3^12 */
    static const seq_t DEN = (seq_t)524288U;   /* 2^19 */

    /* Number of decimal digits to advance: k = n-2 */
    idx_t k = n - 2;

    /* The expansion terminates after 19 decimal digits */
    if (k > 18)
        return 0;

    /* Compute the remainder after the integer part */
    seq_t rem = NUM % DEN;   /* 531441 % 524288 = 7153 */

    /* Generate digits one by one */
    seq_t digit = 0;
    for (idx_t i = 0; i <= k; ++i) {
        rem   = rem * (seq_t)10;
        digit = rem / DEN;
        rem   = rem % DEN;
    }
    return digit;
}
