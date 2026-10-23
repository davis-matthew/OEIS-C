/*
 * A129138  a(n) = number of positive divisors of n that are <= phi(n), where
 *          phi(n) = A000010(n).
 * https://oeis.org/A129138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive divisors of n that are <= φ(n), where
 *            φ is Euler's totient function
 * Range:     u8 n <= 447, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;   /* safety for n = 0, though calls start at 1 */
    }

    /* Compute φ(n) by factorisation */
    idx_t m = n;
    seq_t phi = n;
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            while (m % p == 0) {
                m /= p;
            }
            phi -= phi / (seq_t)p;
        }
    }
    if (m > 1) {
        phi -= phi / (seq_t)m;
    }

    /* Count divisors d of n such that d <= phi */
    seq_t count = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            if ((seq_t)d <= phi) {
                ++count;
            }
            idx_t d2 = n / d;
            if (d2 != d && (seq_t)d2 <= phi) {
                ++count;
            }
        }
    }

    return count;
}
