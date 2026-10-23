/*
 * A118680  Numerator of determinant of n X n matrix with M(i,j) = (i+1)/i if
 *          i=j otherwise 1.
 * https://oeis.org/A118680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = Numerator[ (1 + n(n+1)/2) / n! ]
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute exponent of prime p in n!  (Legendre's formula) */
static uint64_t factorial_prime_exp(idx_t n, uint64_t p) {
    uint64_t exp = 0;
    uint64_t pp = p;
    while (pp <= n) {
        exp += n / pp;
        /* protect against overflow of pp *= p */
        if (pp > n / p) break;
        pp *= p;
    }
    return exp;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n < 1, return 0 for safety */

    /* Compute N = 1 + n(n+1)/2 */
    seq_t N = (seq_t)n * ((seq_t)n + 1) / 2 + 1;

    /* We need gcd(N, n!). Since N ≤ ~n²/2+1, we factor N (small) */
    uint64_t m = (uint64_t)N;          /* safe: N fits 64‑bit for n ≤ 10000 */
    uint64_t G = 1;                    /* G will divide N */

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t eN = 0;
            while (m % p == 0) { m /= p; ++eN; }

            uint64_t eFact = factorial_prime_exp(n, p);
            uint64_t eG = eN < eFact ? eN : eFact;

            for (uint64_t i = 0; i < eG; ++i) G *= p;
        }
    }

    if (m > 1) {  /* remaining prime factor */
        uint64_t p = m;
        uint64_t eFact = factorial_prime_exp(n, p);
        uint64_t eG = (eFact >= 1) ? 1 : 0;
        for (uint64_t i = 0; i < eG; ++i) G *= p;
    }

    /* Return the reduced numerator */
    return N / (seq_t)G;
}
