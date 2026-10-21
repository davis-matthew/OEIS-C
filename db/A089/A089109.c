/*
 * A089109  Convoluted convolved Fibonacci numbers G_5^(r).
 * https://oeis.org/A089109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (66-18*(-1)^n+(115-3*(-1)^n)*n+36*n^2+2*n^3)/48
 * Range:     u8 n <= 1, u16 n <= 26, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 5, a(2) = 9, a(3) = 17, ...  */
seq_t a(idx_t n)
{
    /* Convert n to seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;
    seq_t result;

    /* For even n: (-1)^n = 1
       Numerator = 48 + 112*n + 36*n^2 + 2*n^3   */
    /* For odd  n: (-1)^n = -1
       Numerator = 84 + 118*n + 36*n^2 + 2*n^3   */
    if ((n & 1) == 0) {
        /* even case */
        result = (seq_t)48
               + (seq_t)112 * n_seq
               + (seq_t)36 * n_seq * n_seq
               + (seq_t)2  * n_seq * n_seq * n_seq;
    } else {
        /* odd case */
        result = (seq_t)84
               + (seq_t)118 * n_seq
               + (seq_t)36 * n_seq * n_seq
               + (seq_t)2  * n_seq * n_seq * n_seq;
    }

    /* Divide by 48 – the result is always integer */
    return result / (seq_t)48;
}
