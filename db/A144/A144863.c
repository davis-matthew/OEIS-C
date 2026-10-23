/*
 * A144863  Start with 1, then at each step prepend 10 and append 01.
 * https://oeis.org/A144863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 10000*a(n-1) + 101, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is not defined – return 0 for safety. */
    if (n < 1)
        return (seq_t)0;

    seq_t val = (seq_t)1;          /* a(1) */
    for (idx_t i = 1; i < n; ++i) {/* compute up to a(n) */
        val *= (seq_t)10000;
        val += (seq_t)101;
    }
    return val;
}
