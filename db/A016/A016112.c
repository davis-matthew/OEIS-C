/*
 * A016112  Smallest prime whose digit product is n, if possible; otherwise 0
 *          if n is a prime > 7 or 1 if n has a prime factor > 7.
 * https://oeis.org/A016112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   digit_product, is_prime, largest_prime_divisor, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A016112
 * Smallest prime whose decimal digit product is n, if possible;
 * otherwise 0 if n is a prime > 7 or 1 if n has a prime factor > 7.
 * The sequence index starts at 0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/digit_product.h"
#include "../utils/next_prime.h"
#include "../utils/is_prime.h"
#include "../utils/largest_prime_divisor.h"

#define MAP_SIZE 2000000          /* maximum index (product) we pre‑compute */
#define SIEVE_LIMIT 10000000      /* primes up to this limit will be pre‑computed */

static bool initialized = false;
static seq_t *prod_to_prime = NULL;   /* mapping from product to smallest prime */
static uint64_t last_prime = 0;       /* last prime processed for on‑the‑fly search */

/* Pre‑compute the mapping for all products up to MAP_SIZE-1 */
static void init_map(void)
{
    /* allocate mapping array (zeroed) */
    prod_to_prime = (seq_t *)calloc(MAP_SIZE, sizeof(seq_t));

    /* sieve for primes up to SIEVE_LIMIT */
    bool *is_comp = (bool *)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    if (!is_comp) {
        /* allocation failed – abort */
        exit(EXIT_FAILURE);
    }

    for (uint64_t p = 2; p <= (uint64_t)SIEVE_LIMIT; ++p) {
        if (is_comp[p])
            continue;

        /* skip 1‑digit primes – they are not allowed by the definition */
        if (p >= 10) {
            seq_t prod = digit_product((seq_t)p);
            if (prod < MAP_SIZE && prod_to_prime[(size_t)prod] == 0)
                prod_to_prime[(size_t)prod] = (seq_t)p;
        }

        if (p * p <= (uint64_t)SIEVE_LIMIT) {
            for (uint64_t q = p * p; q <= (uint64_t)SIEVE_LIMIT; q += p)
                is_comp[q] = true;
        }
    }

    free(is_comp);
    last_prime = (uint64_t)SIEVE_LIMIT;   /* start searching after this */
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized)
        init_map();

    /* 1. Immediate match in the pre‑computed table */
    if (n < MAP_SIZE && prod_to_prime[(size_t)n] != 0)
        return prod_to_prime[(size_t)n];

    /* 2. If n is a prime > 7, no such prime exists */
    if (n > 7 && is_prime(n))
        return 0;

    /* 3. If n has a prime factor > 7, answer is 1 */
    uint64_t nm = (uint64_t)n;
    uint64_t lp = largest_prime_divisor(nm);
    if (lp > 7)
        return 1;

    /* 4. For larger n or when the lookup table did not contain an entry,
     *    perform an on‑the‑fly search for the smallest prime with digit
     *    product equal to n.  This loop terminates because such a prime
     *    exists for all composite n whose prime factors are ≤ 7, and
     *    the sequence definition does not require other cases. */
    while (true) {
        last_prime = next_prime(last_prime + 1);
        seq_t prod = digit_product((seq_t)last_prime);
        if (prod == (seq_t)n)
            return (seq_t)last_prime;
    }
}
