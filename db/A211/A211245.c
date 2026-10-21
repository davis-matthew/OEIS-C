/*
 * A211245  Order of 9 mod n-th prime: least k such that prime(n) divides
 *          9^k-1.
 * https://oeis.org/A211245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A062117(n)/gcd(2, A062117(n)) where A062117(n) is the
 *            multiplicative order of 3 modulo the n‑th prime. The result a(n)
 *            is the odd part of that order
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"

/* Computes the n-th term of OEIS sequence A211245 */
seq_t a(idx_t n)
{
    /* Get the n-th prime */
    uint64_t p = get_prime(n);
    /* If 3 divides the prime (p == 3), no such k exists. */
    if (p == 3)
        return (seq_t)0;

    /* Base of exponentiation */
    const seq_t base = (seq_t)3;
    const seq_t mod  = (seq_t)p;

    /* The order of 3 modulo p divides p-1 */
    uint64_t phi = p - 1;

    /* Factorize φ(p) */
    uint64_t pr[32];      /* prime factors */
    uint64_t ex[32];      /* exponents (unused, but factor_u64 requires) */
    size_t   cnt = 0;     /* number of distinct prime factors */

    factor_u64(phi, pr, ex, &cnt);

    /* Compute the multiplicative order of 3 modulo p */
    uint64_t ord = phi;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = pr[i];
        while (ord % q == 0) {
            uint64_t prev_ord = ord;
            ord /= q;
            seq_t r = mod_pow(base, ord, mod);
            if (r == (seq_t)1) {
                /* smaller order found, keep reducing */
                continue;
            } else {
                /* restoring previous order and stop reducing by q */
                ord = prev_ord;
                break;
            }
        }
    }

    /* a(n) is the odd part of the order */
    if ((ord & 1ULL) == 0)
        ord /= 2;
    return (seq_t)ord;
}
