/*
 * A040524  Continued fraction for sqrt(548).
 * https://oeis.org/A040524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0) = 23 For n > 0: a(n) = period[(n-1) mod 14] where period =
 *            [2,2,3,1,5,1,10,1,5,1,3,2,2,46]
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The periodic continued fraction terms for sqrt(548) */
    static const seq_t period[14] = {
        (seq_t)2, (seq_t)2, (seq_t)3, (seq_t)1,
        (seq_t)5, (seq_t)1, (seq_t)10, (seq_t)1,
        (seq_t)5, (seq_t)1, (seq_t)3, (seq_t)2,
        (seq_t)2, (seq_t)46
    };
    if (n == 0)
        return (seq_t)23;                 /* a(0) */
    /* n>=1: use period */
    return period[(n - 1) % (sizeof(period)/sizeof(period[0]))];
}
