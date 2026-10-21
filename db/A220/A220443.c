/*
 * A220443  a(n) = Sum_{i=1..n} (3*i)^2.
 * https://oeis.org/A220443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(1+3*n+2*n^2)/2
 * Range:     u8 n <= 3, u16 n <= 27, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t once to avoid repeated casts. */
    seq_t n_s  = (seq_t)n;
    seq_t a_fac = n_s;
    seq_t b_fac = n_s + (seq_t)1;
    seq_t c_fac = (seq_t)2*n_s + (seq_t)1;

    /* One of the three factors is even; divide that one by 2 to keep
       intermediate values small. */
    if ((a_fac & (seq_t)1) == 0) {
        a_fac /= (seq_t)2;
    } else if ((b_fac & (seq_t)1) == 0) {
        b_fac /= (seq_t)2;
    } else {
        c_fac /= (seq_t)2;
    }

    seq_t result = a_fac * b_fac;
    result *= c_fac;
    result *= (seq_t)3;

    return result;
}
