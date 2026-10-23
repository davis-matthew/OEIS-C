/*
 * A039720  Period of n-countdown club-passing juggling pattern.
 * https://oeis.org/A039720
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = (3+(-1)^n)*(-1+n^2)/2 (equivalently % 2 --> n^2-1 if n
 *            odd, 2*(n^2-1) if n even)
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 2 the sequence is not defined; return 0 as a safe default */
    if (n < 2) {
        return 0;
    }
    seq_t nsq = (seq_t)n * (seq_t)n;   /* n^2 */
    seq_t val = nsq - 1;               /* n^2 - 1 */
    if ((n & 1) == 0) {                /* even n: double the value */
        val *= 2;
    }
    return val;
}
