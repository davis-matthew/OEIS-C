/*
 * A178548  a(n) is the smallest prime p such that q = 2 * p + prime(n+2) is
 *          prime AND A007953(p) = A007953(q) where A007953 is sum of digits.
 * https://oeis.org/A178548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(n) = smallest prime p such that q = 2*p + prime(n+2) is prime
 *            and digit_sum(p) = digit_sum(q)
 * Range:     u8 n <= 2, u16 n <= 5098, u32 n <= 5996, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, digit_sum, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/digit_sum.h"
#include "../utils/is_prime_big.h"
#include "../utils/build_primes_upto.h"

static uint64_t *prime_list = NULL;   /* primes[0] = 2, primes[1] = 3, ... */
static size_t prime_count = 0;        /* number of primes stored */
static idx_t prime_limit = 0;         /* last limit used to build primes */

/* Ensure that prime_list contains at least `need` primes (1‑based). */
static void ensure_primes_at_least(idx_t need)
{
    if (prime_count >= (size_t)need)
        return;

    idx_t new_limit = prime_limit ? prime_limit * 2 : 1000;
    while (prime_count < (size_t)need) {
        uint64_t *list = NULL;
        size_t cnt = 0;
        build_primes_upto(new_limit, &list, &cnt);
        if (prime_list) free(prime_list);
        prime_list = list;
        prime_count = cnt;
        prime_limit = new_limit;
        if (prime_count >= (size_t)need)
            break;
        new_limit *= 2;
    }
}

/* Return the n‑th term of the sequence.  n is 1‑based; n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined but safe */

    /* We need the (n+2)‑th prime: index n+1 in 0‑based array. */
    ensure_primes_at_least(n + 2);
    const idx_t prime_idx_needed = n + 1;          /* 0‑based */
    const uint64_t q_prime_part = prime_list[prime_idx_needed];

    /* Iterate over primes until the condition holds. */
    for (size_t i = 0; ; ++i) {
        /* Make sure the candidate list covers i. */
        if (i >= prime_count)
            ensure_primes_at_least(i + 1);

        uint64_t p = prime_list[i];
        seq_t q = (seq_t)((seq_t)2 * p + q_prime_part);

        if (is_prime_big(q) && digit_sum(p) == digit_sum(q))
            return (seq_t)p;
    }
}
