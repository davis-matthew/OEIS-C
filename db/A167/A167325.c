/*
 * A167325  Totally multiplicative sequence with a(p) = 6*(p+3) for prime p.
 * https://oeis.org/A167325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product_{p^e || n} (6*(p+3))^e (totally multiplicative)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 127, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition; the product over an empty set is 1. */
    if (n <= 1) return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t m = n;

    /* handle factor 2 separately */
    if ((m & (idx_t)1) == 0) {
        idx_t exp = 0;
        while ((m & (idx_t)1) == 0) { m >>= 1; ++exp; }
        if (exp) {
            seq_t factor = (seq_t)6 * ((seq_t)2 + (seq_t)3); /* 30 */
            while (exp--) result *= factor;
        }
    }

    /* odd factors */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) { m /= p; ++exp; }
            seq_t factor = (seq_t)6 * ((seq_t)p + (seq_t)3);
            while (exp--) result *= factor;
        }
    }

    /* remaining prime factor */
    if (m > 1) {
        seq_t factor = (seq_t)6 * ((seq_t)m + (seq_t)3);
        result *= factor;
    }

    return result;
}
