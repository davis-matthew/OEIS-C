/*
 * A167386  a(n) = (-1)^n*n*(n+1)*(2*n-5)/6.
 * https://oeis.org/A167386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n*n*(n+1)*(2*n-5)/6
 * Range:     i8 n <= 4, i16 n <= 25, i32 n <= 1409, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;
    seq_t prod = nn * (nn + 1) * (((seq_t)2 * nn) - (seq_t)5);
    seq_t term = prod / (seq_t)6;
    /* Apply the sign factor: (-1)^n */
    if (n & (idx_t)1) {
        term = -term;
    }
    return term;
}
