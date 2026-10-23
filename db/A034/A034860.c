/*
 * A034860  a(n) = n!*(2*n-5)/2.
 * https://oeis.org/A034860
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * (2*n-5) / 2 (OEIS A034860)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 3 the sequence is undefined; returning 0 is harmless. */
    if (n < 3) return 0;

    /* Compute factorial n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    /* Compute t = 2*n-5 as an integer, then convert to seq_t. */
    idx_t t_int = 2 * n - 5;
    seq_t t = (seq_t)t_int;

    /* Divide by 2 as early as possible to keep intermediate size small. */
    if (t_int & 1) {            /* 2*n-5 is odd: divide factorial first */
        fact /= 2;
        return fact * t;
    } else {                    /* 2*n-5 is even: divide t first */
        return fact * (t / 2);
    }
}
