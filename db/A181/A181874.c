/*
 * A181874  Minute hand closest to hour hand on analog quartz clock. Best
 *          approximation for seconds.
 * https://oeis.org/A181874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, easy
 * Formula:   a(n) = round((300/11)*n) mod 60
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 11;                /* 0..10                */
    idx_t num = (idx_t)r * 300 + 5;  /* integer rounding: floor((300*r+5)/11) */
    idx_t div = num / 11;            /* round((300/11)*r)   */
    idx_t res = div % 60;            /* final value mod 60  */
    return (seq_t)res;               /* cast to output type */
}
