/*
 * A162786  a(n) = A162526(n)/6.
 * https://oeis.org/A162786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + 5 for n ≤ 5, * a(n) = the (n‑1)-th prime for n ≥ 6
 * Range:     u8 n <= 55, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence a(n) = A162526(n)/6   */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* prime_cache[i] = the (i+1)-th prime.  prime_cnt == number of primes stored.  */
static uint64_t *prime_cache = NULL;
static size_t    prime_cnt   = 0;
static size_t    prime_cap   = 0;

/* ensure that prime_cache contains at least target primes */
static void ensure_prime_count(size_t target)
{
    if (target <= prime_cnt)
        return;

    /* first allocation */
    if (prime_cache == NULL) {
        prime_cap = 128;
        prime_cache = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
        if (prime_cache == NULL)
            return;          /* allocation failure – subsequent calls will fail silently */
    }

    uint64_t candidate = (prime_cnt == 0) ? 2 : prime_cache[prime_cnt - 1] + 1;
    while (prime_cnt < target) {
        if (is_prime(candidate)) {
            /* grow cache if full */
            if (prime_cnt == prime_cap) {
                size_t new_cap = prime_cap + prime_cap / 2 + 1;
                uint64_t *tmp = (uint64_t*)realloc(prime_cache,
                                                    new_cap * sizeof(uint64_t));
                if (tmp == NULL)
                    return;    /* allocation failure – stop further growth */
                prime_cache = tmp;
                prime_cap   = new_cap;
            }
            prime_cache[prime_cnt++] = candidate;
        }
        candidate++;
    }
}

seq_t a(idx_t n)
{
    if (n <= 5)                /* first five terms */
        return (seq_t)(n + 5);

    /* primes from the 6th term onward: a(6)=11, a(7)=13, ... */
    size_t target = (size_t)(n - 1);        /* prime index (1‑based) */
    ensure_prime_count(target);
    return (seq_t)prime_cache[target - 1];
}
