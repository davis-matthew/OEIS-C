/*
 * A066750  Greatest common divisor of n and its digit sum.
 * https://oeis.org/A066750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = gcd(n, digit_sum(n))
 * Range:     u8 n <= 409, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the decimal digit sum of n */
    seq_t sum = 0;
    idx_t tmp = n;
    while (tmp != 0) {
        sum += tmp % 10;
        tmp /= 10;
    }

    /* Euclidean algorithm for gcd, using seq_t arithmetic */
    seq_t a_val = (seq_t)n;
    seq_t b_val = sum;
    while (b_val != 0) {
        seq_t r = a_val % b_val;
        a_val = b_val;
        b_val = r;
    }
    return a_val;
}
