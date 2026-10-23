/*
 * A140435  Number of new lattice points created at each step in an n X n grid
 *          that are not visible.
 * https://oeis.org/A140435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for n>1, a(n)=2*n - 2*phi(n) - 1 where phi is Euler's
 *            totient
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) {
        return 0;
    }

    /* Calculate φ(n) via prime factorisation */
    idx_t temp  = n;
    seq_t phi   = (seq_t)n;
    idx_t p     = 2;

    while (p * p <= temp) {
        if (temp % p == 0) {
            /* p is a distinct prime factor of n */
            while (temp % p == 0) {
                temp /= p;
            }
            phi -= phi / (seq_t)p;
        }
        ++p;
    }
    if (temp > 1) {
        /* temp itself is a prime factor > sqrt(original n) */
        phi -= phi / (seq_t)temp;
    }

    /* a(n) = 2*n - 2*phi(n) - 1 */
    seq_t result = (seq_t)2 * (seq_t)n
                 - (seq_t)2 * phi
                 - (seq_t)1;

    return result;
}
