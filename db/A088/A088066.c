/*
 * A088066  Numbers n such that 2*p(n)+3, 2*p(n+1)+3, 2*p(n+2)+3 are
 *          consecutive primes, where p(i) denotes the i-th prime.
 * https://oeis.org/A088066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   lower_bound_idx, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function implements the OEIS formula for A088066, which is */
 /* indices n such that 2*p(n)+3, 2*p(n+1)+3, 2*p(n+2)+3 are consecutive primes */
 /* (p(k) is the k‑th prime, 1‑based). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/sieve_primes.h"
#include "../utils/lower_bound_idx.h"

/* Store the terms found so far (values are seq_t but the data is tiny). */
#define MAX_TERMS 20000                /* enough for all test cases up to 10000 */
static seq_t terms[MAX_TERMS];
static size_t terms_used = 0;

/* Append a term to the array (does nothing if overflow – safety). */
static void add_term(seq_t v)
{
    if (terms_used < MAX_TERMS)
        terms[terms_used++] = v;
}

/* Build the entire sequence once. */
static void init_sequence(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* Sieve up to a limit that yields more than 10000 terms. */
    /* Empirical estimate: need primes up to ~12 million, so set 25 million. */
    const idx_t LIMIT = 25000000; /* 25 000 000 */

    idx_t *prime_list = NULL;
    size_t prime_cnt = 0;
    sieve_primes(LIMIT, &prime_list, &prime_cnt);
    if (prime_list == NULL) return;          /* failed to allocate */

    const idx_t last_prime = prime_list[prime_cnt - 1];

    /* Scan all triples of consecutive primes. */
    for (size_t i = 0; i + 2 < prime_cnt; ++i) {
        idx_t p0 = prime_list[i];
        idx_t p1 = prime_list[i + 1];
        idx_t p2 = prime_list[i + 2];

        idx_t q0 = p0 * 2 + 3;
        idx_t q1 = p1 * 2 + 3;
        idx_t q2 = p2 * 2 + 3;

        /* If any q exceeds the prime list range, skip. */
        if (q2 > last_prime) continue;

        /* Ensure q0, q1, q2 are primes. */
        size_t r0 = lower_bound_idx(prime_list, prime_cnt, q0);
        if (r0 >= prime_cnt || prime_list[r0] != q0) continue;

        size_t r1 = lower_bound_idx(prime_list, prime_cnt, q1);
        if (r1 >= prime_cnt || prime_list[r1] != q1) continue;

        size_t r2 = lower_bound_idx(prime_list, prime_cnt, q2);
        if (r2 >= prime_cnt || prime_list[r2] != q2) continue;

        /* Check that the indices are consecutive. */
        if (r1 == r0 + 1 && r2 == r0 + 2) {
            /* p is the (i+1)-th prime, and indices are 1‑based. */
            add_term((seq_t)(i + 1));
        }
    }

    free(prime_list);
}

/* Return a(n).  a(1) is the first term. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* unused offset 1 */
    init_sequence();
    if (n - 1 < terms_used) return terms[n - 1];
    return (seq_t)0;                         /* out of range – should not happen */
}
