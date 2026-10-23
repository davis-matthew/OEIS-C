/*
 * A048629  n-th 6k+1 prime times n-th 6k-1 prime.
 * https://oeis.org/A048629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (nth prime ≡ 1 mod 6) * (nth prime ≡ 5 mod 6)
 * Range:     u8 n <= 2, u16 n <= 26, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static state for memoization */
static uint64_t *pr1 = NULL;   /* primes congruent to 1 mod 6 */
static uint64_t *pr5 = NULL;   /* primes congruent to 5 mod 6 */
static size_t   len1 = 0, cap1 = 0;
static size_t   len5 = 0, cap5 = 0;
static idx_t    current_limit = 100;   /* initial sieve upper bound */

/* Rebuild the prime lists up to at least 'need' elements in each class */
static void build_to(idx_t need)
{
    if (len1 >= need && len5 >= need) return;

    /* Grow the sieve bound until we have enough primes */
    while (len1 < need || len5 < need) {
        idx_t new_limit = current_limit * 2 + 100;
        size_t sz = (size_t)new_limit + 1u;
        uint8_t *is_prime = (uint8_t*)malloc(sz);
        if (!is_prime) exit(1);            /* allocation failure */

        for (size_t i = 0; i < sz; ++i) is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;

        /* Eratosthenes */
        for (idx_t i = 2; i * i <= new_limit; ++i) {
            if (is_prime[i]) {
                for (idx_t j = i * i; j <= new_limit; j += i) is_prime[j] = 0;
            }
        }

        /* Release previous prime arrays */
        free(pr1);
        free(pr5);
        pr1 = NULL; pr5 = NULL;
        len1 = len5 = 0;
        cap1 = cap5 = 0;

        /* Gather primes of the two residue classes */
        for (idx_t p = 2; p <= new_limit; ++p) {
            if (!is_prime[p]) continue;
            if (p == 2 || p == 3) continue;         /* skip 2,3 */

            if (p % 6 == 1) {
                if (len1 == cap1) {
                    cap1 = cap1 ? cap1 * 2 : 4;
                    pr1 = (uint64_t*)realloc(pr1, cap1 * sizeof(uint64_t));
                }
                pr1[len1++] = p;
            } else if (p % 6 == 5) {
                if (len5 == cap5) {
                    cap5 = cap5 ? cap5 * 2 : 4;
                    pr5 = (uint64_t*)realloc(pr5, cap5 * sizeof(uint64_t));
                }
                pr5[len5++] = p;
            }
        }

        free(is_prime);
        current_limit = new_limit;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset starts at 1, but guard */
    build_to(n);                 /* ensure we have nth primes of each class */
    seq_t prod = (seq_t)pr1[n - 1] * (seq_t)pr5[n - 1];
    return prod;
}
