/*
 * A068823  a(n) = lcm(n, c(n)), where c(n) is the 10's complement of n.
 * https://oeis.org/A068823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look, base
 * Formula:   a(n) = lcm(n, 10^d - n) where d is the decimal digit * length of
 *            n
 * Range:     u8 n <= 10, u16 n <= 100, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the greatest common divisor of two seq_t values. */
static seq_t _gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* convert index to seq_t */
    
    /* smallest power of 10 strictly greater than n */
    seq_t pow10 = 1;
    while (pow10 <= sn)
        pow10 *= 10;
    
    /* 10's complement of n */
    seq_t c = pow10 - sn;
    
    /* gcd(n, c) */
    seq_t g = _gcd_seq_t(sn, c);
    
    /* lcm(n, c) = n/g * c */
    seq_t l = (sn / g) * c;
    
    return l;
}
