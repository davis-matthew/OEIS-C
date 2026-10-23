/*
 * A035216  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          34.
 * https://oeis.org/A035216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(34, d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Legendre symbol (2/p) for odd prime p */
static int legendre_two(uint64_t p) {
    uint64_t r = p & 7ULL;          /* p mod 8 */
    if (r == 1 || r == 7) return 1; /* quadratic residue */
    if (r == 3 || r == 5) return -1;/* quadratic non‑residue */
    return 0;                      /* unused for odd p */
}

/* Precomputed quadratic residues modulo 17 (excluding 0) */
static const bool res17[17] = {
    false, /*0*/
    true,  /*1*/
    true,  /*2*/
    false, /*3*/
    true,  /*4*/
    false, /*5*/
    false, /*6*/
    false, /*7*/
    true,  /*8*/
    true,  /*9*/
    false, /*10*/
    false, /*11*/
    false, /*12*/
    true,  /*13*/
    false, /*14*/
    true,  /*15*/
    true   /*16*/
};

static int legendre_17(uint64_t p) {
    uint64_t r = p % 17ULL;
    if (r == 0) return 0;
    return res17[r] ? 1 : -1;
}

/* Kronecker symbol (34, p) for prime p */
static int kronecker34_prime(uint64_t p) {
    if (p == 2ULL || p == 17ULL) return 0;
    int k2  = legendre_two(p);
    int k17 = legendre_17(p);
    return k2 * k17;
}

/* Value for a prime power p^e */
static seq_t value_prime_power(uint64_t p, uint64_t e) {
    if (p == 2ULL || p == 17ULL) return (seq_t)1;
    int kr = kronecker34_prime(p);
    if (kr == 1) {
        return (seq_t)(e + 1);
    } else if (kr == -1) {
        return (seq_t)(((e & 1ULL) == 0) ? 1 : 0);
    } else {
        return (seq_t)1; /* should not occur for primes */
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, guard against 0 */
    uint64_t m = n;
    seq_t result = 1;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p != 0) continue;
        uint64_t exp = 0;
        while (m % p == 0) {
            m /= p;
            exp++;
        }
        result *= value_prime_power(p, exp);
    }

    if (m > 1) {
        result *= value_prime_power(m, 1);
    }

    return result;
}
