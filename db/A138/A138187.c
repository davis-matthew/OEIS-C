/*
 * A138187  Hankel transform of binomial(2*n+3, n).
 * https://oeis.org/A138187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*floor(n/3)+1 if (n mod 3)=0, -4*(floor(n/3)+1) if (n
 *            mod 3)=1, 2*floor(n/3)+3 if (n mod 3)=2
 * Range:     i8 n <= 96, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    idx_t f = n / 3;            /* floor(n/3) as an integer */

    switch (r) {
        case 0:
            return (seq_t)2 * (seq_t)f + 1;
        case 1: {
            seq_t tmp = (seq_t)4 * ((seq_t)f + 1);
            return -tmp;
        }
        case 2:
            return (seq_t)2 * (seq_t)f + 3;
        /* unreachable */
    }
    return 0; /* to satisfy compiler */
}
