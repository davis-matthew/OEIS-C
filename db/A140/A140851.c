/*
 * A140851  Primes of the form 210k + 53.
 * https://oeis.org/A140851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* OEIS sequence A140851:
   primes of the form 210*k + 53, enumerated for k = 0,1,2,… */

/* dynamically grow an array */
static void *grow_array(void *ptr, size_t *cap, size_t need, size_t elem_size) {
    if (*cap >= need) return ptr;
    size_t new_cap = (*cap == 0) ? 64 : *cap * 2;
    while (new_cap < need) new_cap *= 2;
    void *np = realloc(ptr, new_cap * elem_size);
    if (!np) abort();
    *cap = new_cap;
    return np;
}

/* static cache of primes of the required form */
static seq_t *prime_cache = NULL;     /* array of found primes          */
static size_t prime_cap   = 0;        /* allocated size of prime_cache  */
static size_t prime_cnt   = 0;        /* number of primes stored        */
static idx_t   next_k     = 0;        /* next k to test (candidate = 210*k+53) */

/* generate primes until we have at least target primes stored */
static void generate_to(idx_t target) {
    while (prime_cnt < target) {
        seq_t cand = (seq_t)(210 * next_k + 53);   /* 210*k + 53 */
        if (is_prime_big(cand)) {
            prime_cache = (seq_t *)grow_array(prime_cache, &prime_cap,
                                              prime_cnt + 1, sizeof(seq_t));
            prime_cache[prime_cnt++] = cand;
        }
        ++next_k;
    }
}

/* OEIS sequence A140851 */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n < 1, return 0 as a safe value */
    generate_to(n);
    return prime_cache[n - 1];
}
