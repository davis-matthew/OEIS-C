/*
 * A119589  Natural numbers from 1 to 100 sorted lexicographically.
 * https://oeis.org/A119589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(n) = if n < 4 then 10^(n-1) else let m = n+8, q = m/11, r =
 *            m%11; if r < 10 then 10*q + r else q + 1 (according to OEIS
 *            A119589)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle first three terms explicitly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)10;
    if (n == 3) return (seq_t)100;

    /* for n >= 4 */
    seq_t m = (seq_t)n + (seq_t)8;   /* n+8 */
    seq_t q = m / (seq_t)11;        /* quotient in base-11 */
    seq_t r = m % (seq_t)11;        /* remainder in base-11 */

    if (r < (seq_t)10) {
        return (seq_t)10 * q + r;
    } else {
        return q + (seq_t)1;
    }
}
