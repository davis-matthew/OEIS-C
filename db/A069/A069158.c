/*
 * A069158  a(n) = Product_{d|n} mu(d), product over positive divisors, d, of
 *          n, where mu(d) = Moebius function (A008683).
 * https://oeis.org/A069158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Π_{d|n} μ(d)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A069158: product of Moebius values over all divisors of n. */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Special case n = 1 */
    if (n == 1) return 1;

    /* Work on the 64‑bit representation of n */
    uint64_t m = (uint64_t)n;
    int primeCount = 0;      /* number of distinct prime factors (squarefree case) */
    int repeated = 0;        /* flag: true if any prime occurs with exponent ≥ 2 */

    /* Handle factor 2 */
    if ((m & 1ULL) == 0) {
        int e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            e++;
        }
        if (e >= 2) repeated = 1;
        else primeCount++;
    }

    /* Trial division for odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (repeated) break;          /* already zero */
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            if (e >= 2) repeated = 1;
            else primeCount++;
        }
    }

    /* Remaining prime factor (if any) */
    if (!repeated && m > 1) {
        primeCount++;
    }

    /* Result according to the analysis above */
    if (repeated) return 0;
    if (primeCount == 1) return -1;
    return 1;
}
