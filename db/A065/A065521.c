/*
 * A065521  a(n) = floor(prime(n) / n) * n - prime(n) mod n.
 * https://oeis.org/A065521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(prime(n)/n)*n - prime(n) mod n
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t *primitive_primes = NULL;
static size_t num_primes = 0;
static size_t prime_cap = 0;
static idx_t next_candidate = 2;

/* Ensure that the first n primes are generated */
static void ensure_primes(idx_t n)
{
    while (num_primes < n) {
        idx_t cand = next_candidate;
        int is_prime = 1;

        /* test divisibility by previously found primes */
        for (size_t i = 0; i < num_primes; ++i) {
            idx_t p = primitive_primes[i];
            if ((idx_t)p * (idx_t)p > cand)
                break;
            if (cand % p == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            if (num_primes == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                idx_t *tmp = (idx_t *)realloc(primitive_primes,
                                              new_cap * sizeof(idx_t));
                if (!tmp) abort();
                primitive_primes = tmp;
                prime_cap = new_cap;
            }
            primitive_primes[num_primes++] = cand;
        }

        /* Advance to next candidate: after 2 we skip even numbers */
        if (cand == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }
}

seq_t a(idx_t n)
{
    ensure_primes(n);
    seq_t p = (seq_t)primitive_primes[n - 1];
    seq_t nn = (seq_t)n;
    seq_t k = p / nn;
    seq_t rem = p % nn;
    return k * nn - rem;
}
