/*
 * A069879  Number of pairs {i,j} with i different from j; 1<=i<=n; 1<= j <=n
 *          such that i+j is a prime number.
 * https://oeis.org/A069879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * A071917(n) (number of ordered distinct pairs (i,j),
 *            1≤i≠j≤n, with i+j prime)
 * Range:     u8 n <= 31, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Return the number of unordered prime‑sum pairs multiplied by 2.
   The implementation keeps a prime sieve up to 2*max_seen_n and
   re‑uses it for all subsequent calls.  */
seq_t a(idx_t n)
{
    /* static data: sieve up to 2*max_n, stored primes */
    static idx_t max_precomp = 0;          /* upper bound of sieve (exclusive)  */
    static uint32_t *primes = NULL;        /* array of primes up to max_precomp-1 */
    static size_t prime_cnt = 0;           /* number of primes in the array */

    /* extend sieve when larger n is requested */
    if (n > 0 && (idx_t)(2 * n + 1) > max_precomp) {
        idx_t new_max = 2 * n + 1;        /* allocate up to 2*n inclusive */
        uint8_t *is_prime = (uint8_t *)malloc((size_t)new_max);
        if (!is_prime) return 0;          /* allocation failure → 0 */

        /* simple Eratosthenes sieve */
        memset(is_prime, 1, (size_t)new_max);
        is_prime[0] = is_prime[1] = 0;
        for (idx_t p = 2; p * p < new_max; ++p) {
            if (is_prime[p]) {
                for (idx_t q = p * p; q < new_max; q += p)
                    is_prime[q] = 0;
            }
        }

        /* count primes and store them */
        size_t cnt = 0;
        for (idx_t i = 2; i < new_max; ++i)
            if (is_prime[i]) ++cnt;

        uint32_t *new_primes = (uint32_t *)realloc(primes, cnt * sizeof(uint32_t));
        if (!new_primes) {
            free(is_prime);
            return 0;              /* allocation failure → 0 */
        }
        primes = new_primes;
        prime_cnt = cnt;

        idx_t idxp = 0;
        for (idx_t i = 2; i < new_max; ++i) {
            if (is_prime[i]) {
                primes[idxp++] = (uint32_t)i;
            }
        }

        max_precomp = new_max;
        free(is_prime);
    }

    seq_t total = 0;
    idx_t limit = 2 * n;                 /* maximum possible sum */

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint32_t p = primes[pi];
        if (p > limit) break;

        idx_t low  = (p > n) ? (p - n) : (idx_t)1;
        idx_t high = (p - 1 < n) ? (p - 1) : n;
        idx_t half = (p - 1) / 2;            /* floor((p-1)/2) */

        idx_t imax = (high < half) ? high : half;
        if (imax >= low) {
            total += (seq_t)(imax - low + 1);
        }
    }

    total *= (seq_t)2;   /* ordered pairs i≠j */
    return total;
}
