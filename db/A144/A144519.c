/*
 * A144519  Triangular numbers n*(n+1)/2 with n prime and n+1 nonprime.
 * https://oeis.org/A144519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p_{n+1})*(p_{n+1}+1)/2 where p_k is the k‑th prime
 *            (excluding 2). Since 2 is the only prime with p+1 prime, this is
 *            simply the triangular number of the n‑th odd prime
 * Range:     u8 n <= 7, u16 n <= 71, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------- prime generation helpers ------------------------------------ */
static uint64_t *prime_list = NULL;   /* dynamic array of primes */
static size_t prime_cnt   = 0;         /* number of primes stored */
static uint64_t next_candidate = 3;    /* next odd number to test */
static size_t prime_cap    = 0;        /* capacity of prime_list */

/* Ensure we have at least 'target' primes in prime_list.  */
static void
ensure_primes(size_t target)
{
    if (prime_cnt >= target) return;

    if (prime_list == NULL) {
        prime_cap = 128;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (prime_list == NULL) exit(1);
        prime_list[0] = 2;                /* store 2 as first prime */
        prime_cnt = 1;
        next_candidate = 3;
    }

    while (prime_cnt < target) {
        bool is_prime = true;
        /* test divisibility by known primes up to sqrt(candidate) */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            __uint128_t prod = (__uint128_t)p * p;
            if (prod > next_candidate) break;
            if (next_candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                    prime_cap * sizeof(uint64_t));
                if (tmp == NULL) exit(1);
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = next_candidate;
        }
        next_candidate += 2;          /* skip even numbers */
    }
}

/* ------- sequence implementation ------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* behavior below the offset is unspecified */

    /* term n uses the (n+1)-th prime; prime_list[0] is 2, so we need prime_list[n] */
    size_t target_index = (size_t)n + 1;   /* count of primes required */
    ensure_primes(target_index);

    uint64_t p = prime_list[n];           /* nth odd prime (skip 2) */
    seq_t tp = (seq_t)p * ((seq_t)p + 1u) / 2u;
    return tp;
}
