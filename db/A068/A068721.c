/*
 * A068721  Arithmetic derivative of cubes: a(n) = 3*n^2*A003415(n).
 * https://oeis.org/A068721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^2*A003415(n) (A003415 is the arithmetic derivative of
 *            n)
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 767, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t d = 0;            /* arithmetic derivative n' */
    unsigned long long tmp = n;

    /* factor n by trial division (sufficient for n up to at least 10 000 and beyond) */
    for (unsigned long long p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            unsigned int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            /* term = e * n / p   (integer because n is divisible by p) */
            d += (seq_t)e * (n_seq / (seq_t)p);
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        d += (seq_t)1 * (n_seq / (seq_t)tmp);
    }

    seq_t result = 3;
    result *= n_seq;
    result *= n_seq;
    result *= d;
    return result;
}
