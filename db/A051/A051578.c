/*
 * A051578  a(n) = (2*n+4)!!/4!!, related to A000165 (even double factorials).
 * https://oeis.org/A051578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+2)! * 2^(n-1) Recurrence: a(0) = 1, a(n+1) = (2*n + 6)
 *            * a(n) for n >= 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base value a(0) = 1 */
    if (n == 0) return (seq_t)1;

    seq_t val = (seq_t)1;           /* a(0) */
    for (idx_t i = 0; i < n; ++i) {
        /* multiply by (2*i + 6) to get a(i+1) from a(i)      */
        seq_t mult = (seq_t)(2 * i + 6);
        val *= mult;                  /* a(i+1) = mult * a(i)         */
    }
    return val;                      /* val now equals a(n)             */
}
