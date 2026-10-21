/*
 * A070614  a(n) = n^5 mod 31.
 * https://oeis.org/A070614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 31
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 31 first – the reduced value fits in seq_t */
    uint64_t r = n % 31U;
    seq_t x = (seq_t)r;

    /* Compute x^5 modulo 31 using only small intermediate values */
    seq_t x2  = (x * x) % (seq_t)31;   /* x^2  (<= 900)      */
    seq_t x4  = (x2 * x2) % (seq_t)31; /* x^4  (<= 900)      */
    seq_t x5  = (x4 * x)  % (seq_t)31; /* x^5  (<= 27000)    */

    return x5;
}
