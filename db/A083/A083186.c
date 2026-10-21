/*
 * A083186  Sum of the first n primes whose indices are primes.
 * https://oeis.org/A083186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} prime(prime(k))
 * Range:     u8 n <= 8, u16 n <= 69, u32 n <= 8475, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/sieve_primes.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 // offset starts at 1, but guard

    /* static state: primes list, sequence cache, and running info */
    static bool primes_inited = false;
    static idx_t *prime_list = NULL;   /* array of all primes up to limit */
    static size_t prime_cnt = 0;       /* number of primes in array */
    static const idx_t PRIMES_LIMIT = 2000000;  /* enough for n <= 10000 */

    static bool cache_inited = false;
    static seq_t *seq_cache = NULL;   /* cumulative sums of terms */
    static size_t cache_cap = 0;      /* allocated size of seq_cache */
    static size_t computed_terms = 0; /* how many terms already computed */
    static idx_t prime_index = 0;     /* index of next prime to examine */
    static seq_t running_sum = 0;     /* sum of the terms computed so far */

    /* Initialize primes once */
    if (!primes_inited) {
        sieve_primes(PRIMES_LIMIT, &prime_list, &prime_cnt);
        primes_inited = true;
    }

    /* Expand cache to hold at least n terms */
    if (!cache_inited || cache_cap < (size_t)n) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* out‑of‑memory */
        seq_cache = tmp;
        cache_cap = new_cap;
        cache_inited = true;
    }

    /* Compute missing terms up to n */
    size_t target = (size_t)n;          /* target number of terms */
    while (computed_terms < target) {
        /* Ensure we still have primes to examine */
        if ((idx_t)prime_index >= (idx_t)prime_cnt) {
            /* Should not happen for n <= 10000 with the chosen limit */
            abort();
        }

        idx_t current_prime_pos = (idx_t)prime_index + 1;  /* 1‑based index */
        if (is_prime(current_prime_pos)) {                /* prime index ? */
            running_sum += (seq_t)prime_list[prime_index];
            seq_cache[computed_terms++] = running_sum;
        }
        prime_index++;
    }

    return seq_cache[n - 1];
}
