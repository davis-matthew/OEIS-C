/*
 * A178181  Minute with hour hand overlap problem on analog clock.
 * https://oeis.org/A178181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = floor((720*n)/11) mod 60
 * Range:     u8 n <= 0, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (720 * n) / 11 in integer arithmetic and take the remainder
       modulo 60.  This works for all n >= 0 and yields the pattern
       0,5,10,16,21,27,32,38,43,49,54,0,... which repeats every 11 hours. */
    seq_t val = (seq_t)n * (seq_t)720;   /* 720 * n */
    val = val / (seq_t)11;               /* floor division */
    return val % (seq_t)60;
}
