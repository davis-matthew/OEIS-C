/*
 * A085983  Number of 9's in decimal expansion of prime(n).
 * https://oeis.org/A085983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of decimal digits 9 in the n‑th prime p_n (p_1 =
 *            2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_9s, upper_bound_prime_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>
#include "../utils/count_9s.h"
#include "../utils/upper_bound_prime_idx.h"

/* ------- reusable data --------------------------------------------------- */
static uint64_t *prime_cache = NULL;      /* primes gathered so far          */
static size_t   prime_cnt   = 0;          /* number of stored primes         */
static size_t   prime_cap   = 0;          /* capacity of prime_cache         */

static unsigned char *is_composite = NULL;  /* sieve marks                 */
static size_t        comp_cap     = 0;      /* capacity of is_composite      */

/* ------- rebuild (or enlarge) the sieve --------------------------------- */
static void build_sieve(uint64_t limit)
{
    /* Reallocate composite array */
    size_t needed_cap = limit + 1;
    if (comp_cap < needed_cap) {
        unsigned char *tmp = (unsigned char*)realloc(is_composite,
                                                    needed_cap * sizeof(unsigned char));
        if (!tmp) abort();
        is_composite = tmp;
        comp_cap = needed_cap;
    }
    /* clear entries up to limit */
    for (size_t i = 0; i <= limit; ++i) is_composite[i] = 0;

    /* reset prime list */
    prime_cnt = 0;
    /* sieve */
    for (uint64_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            /* prime detected */
            if (prime_cnt >= prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 32;
                uint64_t *tmp = (uint64_t*)realloc(prime_cache,
                                                   new_cap * sizeof(uint64_t));
                if (!tmp) abort();
                prime_cache = tmp;
                prime_cap = new_cap;
            }
            prime_cache[prime_cnt++] = i;
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    is_composite[j] = 1;
            }
        }
    }
}

/* ------- public sequence function --------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)  /* the sequence starts at offset 1; defensive return 0 */
        return (seq_t)0;

    /* ensure the sieve contains at least n primes */
    if ((uint64_t)prime_cnt < (uint64_t)n) {
        uint64_t lim = upper_bound_prime_idx(n);
        build_sieve(lim);
        /* in the very unlikely event the bound was too low, grow */
        if ((uint64_t)prime_cnt < (uint64_t)n)
            build_sieve(lim * 2);
    }

    uint64_t pn = prime_cache[n - 1];          /* nth prime (1‑based) */
    return count_9s(pn);
}
