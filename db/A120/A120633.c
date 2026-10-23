/*
 * A120633  Number of composite numbers bounded inclusively between p(n+1) and
 *          2*p(n) where p(x) is prime(x).
 * https://oeis.org/A120633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by sieving numbers up to 2·p(n) and counting the composites
   in the inclusive interval [p(n+1), 2·p(n)]. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*---------------------  Sieve state  ---------------------*/
static size_t sieve_limit = 0;        /* largest n that has been sieved */
static uint8_t *is_prime_flag = NULL;/* 1 = prime, 0 = composite          */
static uint32_t *prime_prefix = NULL;/* prime_prefix[i] = #primes ≤ i      */
static uint64_t *primes = NULL;      /* list of primes                    */
static size_t primes_count = 0;
static size_t primes_capacity = 0;

/* Extend the sieve up to 'limit'; grows arrays as necessary. */
static void extend_sieve_to(size_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* Resize flag array */
    uint8_t *nf = (uint8_t*)realloc(is_prime_flag,
                                   (limit + 1) * sizeof(uint8_t));
    if (!nf) exit(1);
    is_prime_flag = nf;

    /* Resize prefix array */
    uint32_t *np = (uint32_t*)realloc(prime_prefix,
                                     (limit + 1) * sizeof(uint32_t));
    if (!np) exit(1);
    prime_prefix = np;

    /* Initialise prefix for index 0 (and 1 if present). */
    if (sieve_limit == 0) {
        prime_prefix[0] = 0;
        if (limit >= 1)
            prime_prefix[1] = 0;
    }

    /* Sieve the new range [sieve_limit+1, limit] */
    for (size_t i = sieve_limit + 1; i <= limit; ++i) {
        bool prime = true;
        if (i < 2) {
            prime = false;
        } else {
            for (size_t pj = 0; pj < primes_count; ++pj) {
                uint64_t p = primes[pj];
                if (p > i / p)      /* p*p > i */
                    break;
                if (i % p == 0) {
                    prime = false;
                    break;
                }
            }
        }
        is_prime_flag[i] = prime ? 1 : 0;

        uint32_t prev = (i > 0) ? prime_prefix[i - 1] : 0;
        prime_prefix[i] = prev + (prime ? 1 : 0);

        if (prime) {
            if (primes_count == primes_capacity) {
                size_t new_cap = primes_capacity ? primes_capacity * 2 : 16;
                uint64_t *np_ = (uint64_t*)realloc(primes,
                                                  new_cap * sizeof(uint64_t));
                if (!np_) exit(1);
                primes = np_;
                primes_capacity = new_cap;
            }
            primes[primes_count++] = i;
        }
    }

    sieve_limit = limit;
}

/* Ensure that at least 'needed' primes are available. */
static void ensure_primes_count(size_t needed)
{
    while (primes_count < needed) {
        size_t new_limit = (sieve_limit == 0) ? 100 : sieve_limit * 2;
        extend_sieve_to(new_limit);
    }
}

/* Ensure prime_prefix is available up to 'limit'. */
static void ensure_prime_prefix_up_to(size_t limit)
{
    extend_sieve_to(limit);
}

/*-----------------------------------------  Sequence  -----------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* offset is 1; behaviour below does not matter */

    /* Primes up to p(n+1) are required. */
    ensure_primes_count((size_t)n + 1);

    /* p(n) and p(n+1) */
    uint64_t p_n      = primes[(size_t)(n - 1)]; /* prime index starts at 0 */
    uint64_t p_next   = primes[(size_t)(n)    ];

    /* Upper bound 2·p(n) */
    size_t upper_limit = (size_t)(p_n * 2ULL);

    /* Build sieve up to the needed range. */
    ensure_prime_prefix_up_to(upper_limit);

    size_t lower = (size_t)p_next;
    size_t upper = upper_limit;

    size_t total_numbers = upper - lower + 1;
    uint32_t primes_in_range = prime_prefix[upper] - prime_prefix[lower - 1];
    size_t composites = total_numbers - primes_in_range;

    return (seq_t)composites;
}
