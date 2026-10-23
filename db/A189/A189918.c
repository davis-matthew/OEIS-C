/*
 * A189918  Sum of tetrahedral numbers A000292(k), with k in the reduced
 *          residue system modulo n.
 * https://oeis.org/A189918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+2)*(n*(n+2)+mu(rad(n))*rad(n))*phi(n))/24, n≥2,
 *            a(1)=0
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 449, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return 0;
    }

    idx_t tmp = n;
    seq_t phi = n;          /* φ(n)   */
    seq_t rad = 1;          /* product of distinct prime factors */
    int   omega = 0;        /* number of distinct prime factors */

    /* factor 2 separately */
    if ((tmp & 1) == 0) {
        phi -= phi / 2;
        rad *= 2;
        omega++;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
        }
    }

    /* trial division for odd primes */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            phi -= phi / p;
            rad *= p;
            omega++;
            while (tmp % p == 0) {
                tmp /= p;
            }
        }
    }

    /* remaining prime factor > sqrt(tmp) */
    if (tmp > 1) {
        phi -= phi / tmp;
        rad *= tmp;
        omega++;
    }

    /* mu(rad(n)) = (-1)^ω(n) */
    int mu_sign = (omega % 2 == 0) ? 1 : -1;

    /* part1 = n*(n+2) */
    seq_t part1 = (seq_t)n * (seq_t)(n + 2);

    /* part2 = part1 + mu*rad */
    seq_t part2 = (mu_sign == 1) ? part1 + rad : part1 - rad;

    /* numerator = (n+2) * part2 * phi */
    seq_t numerator = (seq_t)(n + 2) * part2 * phi;

    /* final result */
    seq_t result = numerator / 24;

    return result;
}
