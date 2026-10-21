/*
 * A085731  Greatest common divisor of n and its arithmetic derivative.
 * https://oeis.org/A085731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = GCD(n, A003415(n))
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (Arithmetic derivative of n)            */
/* Computation: factor n, then use the multiplicative rule: */
/*   a(p^e) = p^e if p divides e, otherwise p^(e-1).          */

#include <stdint.h>
#include <stddef.h>

/* Multiply a seq_t by p ^ exp (exp >= 0). */
static seq_t pow_seq_t(seq_t p, unsigned int exp) {
    seq_t r = 1;
    for (unsigned int i = 0; i < exp; ++i) r *= p;
    return r;
}

/* Factorise n into primes and exponents; primes[] and exp[] are
 * assumed to be large enough (size 64 is safe for all n up to 2^64). */
static void factor_uint64(uint64_t n, uint64_t *primes, int *exp, size_t *cnt) {
    size_t c = 0;
    if (n <= 1) { *cnt = 0; return; }
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; p += (p==2?1:2)) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            primes[c] = p;
            exp[c++] = e;
        }
    }
    if (m > 1) {              /* remaining prime factor */
        primes[c] = m;
        exp[c++] = 1;
    }
    *cnt = c;
}

/* Main function */
seq_t a(idx_t n) {
    if (n <= 1) return 1;           /* gcd(1,0)=1 */

    uint64_t primes[64];
    int exp[64];
    size_t cnt = 0;

    factor_uint64((uint64_t)n, primes, exp, &cnt);

    seq_t g = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        unsigned int e = (unsigned int)exp[i];
        unsigned int k = (e % p == 0) ? e : e - 1;   /* exponent for the gcd */
        g *= pow_seq_t((seq_t)p, k);
    }
    return g;
}
