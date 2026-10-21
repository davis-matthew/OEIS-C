/*
 * A086001  For p = prime(n), a(n) is the smallest k such that p*(1 + 2
 *          Ord(4,p) k) is a base-2 pseudoprime.
 * https://oeis.org/A086001
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest k ≥ 1 such that for p = prime(n) p*(1 +
 *            2*Ord(4,p)*k) is a composite base‑2 pseudoprime. Offset: 2 (a(2)
 *            is the first term listed)
 * Range:     u8 none, u16 n <= 22, u32 n <= 1136, u64 n <= 9158,
 *            u128 n <= 9158, big any n
 * Helpers:   factorize, get_prime, is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"        /* get_prime(idx_t) */
#include "../utils/factorize.h"        /* factorize(uint64_t, uint64_t[], int[]) */
#include "../utils/is_prime_big.h"     /* is_prime_big(seq_t) */
#include "../utils/mod_pow.h"          /* mod_pow(seq_t, uint64_t, seq_t) */

/* Compute the multiplicative order of 4 modulo the odd prime p.
   The function returns the order as a seq_t (small enough to fit
   in the integer representation of seq_t). */
static seq_t order_of_4(uint64_t p)
{
    uint64_t ord = p - 1;                      /* start with phi(p) */
    uint64_t phi = ord;

    uint64_t fac[64];
    int exp[64];
    size_t cnt = factorize(phi, fac, exp);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t prime = fac[i];
        while ((ord % prime == 0) &&
               (mod_pow((seq_t)4, ord / prime, (seq_t)p) == (seq_t)1))
            ord /= prime;
    }
    return (seq_t)ord;
}

/* Core sequence function. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                     /* undefined for n < 2 */

    seq_t p = get_prime(n);                  /* nth prime (1‑based) */
    seq_t t = 2 * order_of_4((uint64_t)p);   /* 2 * Ord(4,p) */

    seq_t k = 1;
    while (1) {
        seq_t q = 1 + t * k;                 /* q = 1 + t*k */
        seq_t m = p * q;                     /* m = p*(1 + t*k) */

        /* m must be composite and a base‑2 pseudoprime */
        if (!is_prime_big(m)) {
            /* exponent fits into 64‑bit for the intended range */
            uint64_t exp = (uint64_t)m - 1;
            if (mod_pow((seq_t)2, exp, m) == (seq_t)1)
                return k;
        }
        ++k;
    }
}
