/*
 * A133937  A100994-n/A014963.
 * https://oeis.org/A133937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = φ(n) if n is a prime power, otherwise a(n) = -(n-1); a(1)
 *            = 0
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)0;
    }

    idx_t temp = n;
    seq_t phi = (seq_t)n;          /* start with φ(n) = n */
    int distinct = 0;              /* number of distinct prime factors */

    /* trial division up to sqrt(n) */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            ++distinct;
            /* phi = phi / p * (p-1) */
            phi = phi / (seq_t)p * ((seq_t)p - 1);
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        ++distinct;
        idx_t p = temp;
        phi = phi / (seq_t)p * ((seq_t)p - 1);
    }

    if (distinct == 1) {   /* prime or prime power */
        return phi;
    } else {
        return -((seq_t)n - 1);
    }
}
