/*
 * A021285  Decimal expansion of 1/281.
 * https://oeis.org/A021285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) is the nth decimal digit of 1/281 computed via long
 *            division
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The remainder after each division step is always < 281.
       Using idx_t keeps it large enough irrespective of seq_t size. */
    idx_t r = 1;
    seq_t digit = 0;

    for (idx_t i = 0; i <= n; ++i) {
        idx_t t = r * (idx_t)10;          /* multiply with 10 once */
        digit = (seq_t)(t / (idx_t)281);  /* integer part → digit */
        r = t % (idx_t)281;               /* new remainder  */
    }
    return digit;
}
