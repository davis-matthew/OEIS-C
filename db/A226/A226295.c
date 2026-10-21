/*
 * A226295  Multiplicative order of the n-th prime modulo the (n+1)th prime.
 * https://oeis.org/A226295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = multiplicative order of the * n-th prime modulo the
 *            (n+1)-th prime
 * Range:     u8 n <= 53, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mod_pow.h"          /* mod_pow(seq_t base, uint64_t exp, seq_t mod) */
#include "../utils/get_prime.h"       /* uint32_t get_prime(idx_t k) */
#include "../utils/build_primes_upto.h" /* build_primes_upto(idx_t limit, uint64_t **out, size_t *cnt) */

seq_t a(idx_t n)
{
    /* Get the n-th and (n+1)-th primes */
    seq_t p = (seq_t)get_prime(n);          /* p_n */
    seq_t q = (seq_t)get_prime(n + 1);      /* p_{n+1} */

    seq_t m = q - 1;           /* φ(p_{n+1}) = p_{n+1} - 1  */
    seq_t order = m;           /* initial candidate for the order */

    /* Prime factorization of m (m <= 200000 for n <= 10000) */
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0;
    if (!prime_list) {
        /* Build a small list of primes for factorization; 200000 is safely
           above the square‑root of any m we will encounter. */
        build_primes_upto(200000, &prime_list, &prime_cnt);
    }

    seq_t temp = m;

    /* For each prime divisor p of m, try to reduce the candidate order. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t pr = (seq_t)prime_list[i];
        if (pr * pr > temp)  break;
        if (temp % pr == 0) {
            /* Remove all powers of pr from temp */
            while (temp % pr == 0) temp /= pr;

            /* Reduce order by pr as much as possible */
            while (order % pr == 0 &&
                   mod_pow(p, (uint64_t)(order / pr), q) == 1)
            {
                order /= pr;
            }
        }
    }

    /* If a prime factor larger than sqrt(m) remains */
    if (temp > 1) {
        seq_t pr = temp;  /* the remaining prime factor */
        while (order % pr == 0 &&
               mod_pow(p, (uint64_t)(order / pr), q) == 1)
        {
            order /= pr;
        }
    }

    return order;
}
