/*
 * A188902  Numerator of the base n logarithm of the product of the divisors
 *          of n.
 * https://oeis.org/A188902
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = numerator( d(n) / 2 )
 * where d(n) is the number of divisors of n.
 * If d(n) is even, the result is d(n)/2, otherwise it is d(n).
 */
seq_t a(idx_t n)
{
    if (n < 2) {
        return (seq_t)0;     /* value undefined for n < 2, return 0 */
    }

    idx_t m = n;
    seq_t divcnt = 1;        /* start with multiplicative identity */

    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            divcnt *= (seq_t)(exp + 1);
        }
    }

    if (m > 1) {            /* remaining prime factor */
        divcnt *= (seq_t)2;
    }

    if ((divcnt & (seq_t)1) == 0) {
        return divcnt >> 1; /* d(n) is even */
    } else {
        return divcnt;     /* d(n) is odd */
    }
}
