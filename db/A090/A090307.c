/*
 * A090307  a(n) = 18*a(n-1) + a(n-2), starting with a(0) = 2 and a(1) = 18.
 * https://oeis.org/A090307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 18*a(n-1) + a(n-2), a(0) = 2, a(1) = 18
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)18;

    seq_t prev2 = (seq_t)2;   /* a(0) */
    seq_t prev1 = (seq_t)18;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        /* Use the recurrence; arithmetic is exact until the value overflows
           the width of the type, beyond which the term is not required. */
        seq_t cur = prev1 * (seq_t)18 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
