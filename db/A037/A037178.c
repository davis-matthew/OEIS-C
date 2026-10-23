/*
 * A037178  Longest cycle when squaring modulo n-th prime.
 * https://oeis.org/A037178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = order of 2 modulo k, where k = odd part of (prime(n) - 1)
 * Range:     u8 n <= 54, u16 n <= 6725, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, factorize, get_prime, mod_pow, odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation note: 
 *  For each prime p = prime(n) we compute k = odd_part(p-1).  
 *  Then we find the multiplicative order of 2 modulo k.  
 *  The order divides φ(k), so we factor φ(k) and repeatedly test  
 *  divisibility by each prime factor until no further reduction is possible. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/get_prime.h"      /* get_prime(idx_t) -> uint32_t */
#include "../utils/odd_part.h"       /* odd_part(seq_t) -> seq_t */
#include "../utils/calc_phi.h"       /* calc_phi(seq_t) -> seq_t */
#include "../utils/factorize.h"     /* factorize(uint64_t, uint64_t*, int*) -> size_t */
#include "../utils/mod_pow.h"       /* mod_pow(seq_t, uint64_t, seq_t) -> seq_t */

/* Compute A037178: the multiplicative order of 2 modulo the odd part of (prime(n)-1). */
seq_t a(idx_t n)
{
    /* get the n-th unsigned prime */
    uint32_t p_uint32 = get_prime(n);
    seq_t p = (seq_t)p_uint32;

    /* compute odd part of p-1 */
    seq_t k_seq = odd_part(p - 1);
    uint64_t k = (uint64_t)k_seq;          /* k fits into 64 bits for the expected n-range */

    /* special case: order of 2 modulo 1 is defined as 1 */
    if (k == 1) return (seq_t)1;

    /* totient of k */
    seq_t phi_seq = calc_phi(k_seq);
    uint64_t phi = (uint64_t)phi_seq;     /* phi(k) <= k-1 <= 1e5 for the relevant range */

    /* factorize phi to obtain its prime divisors */
    uint64_t primes[64];
    int exps[64];                /* exponents are unused, but factorize wants them */
    size_t cnt = factorize(phi, primes, exps);

    /* start with the maximum possible order: phi */
    uint64_t ord = phi;

    /* Try to reduce ord by each prime factor of phi */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t f = primes[i];
        while (ord % f == 0) {
            uint64_t test = ord / f;
            /* if 2^test ≡ 1 (mod k), then we can reduce the order */
            if (mod_pow((seq_t)2, test, k_seq) == (seq_t)1) {
                ord = test;
            } else {
                break;
            }
        }
    }

    return (seq_t)ord;
}
