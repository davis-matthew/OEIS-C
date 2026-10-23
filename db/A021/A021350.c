/*
 * A021350  Decimal expansion of 1/346.
 * https://oeis.org/A021350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the nth decimal digit of the fraction 1/346 * computed by
 *            a standard long‑division algorithm. * Each call performs O(n)
 *            arithmetic on small integers (≤ 346), * which is well within the
 *            time limits for n up to about 10000
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t DEN = 346;   /* denominator of the fraction */
    seq_t rem = 1;           /* current remainder before division */
    seq_t digit = 0;         /* current digit produced */

    /* Produce the (n+1)-st digit after the decimal point
     * (i.e., a(0) is the first digit). */
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;           /* bring down a zero */
        digit = rem / DEN;   /* integer division gives next digit */
        rem %= DEN;          /* remainder for next step */
    }
    return digit;
}
