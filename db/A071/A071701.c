/*
 * A071701  Number of twin prime pairs <= n of form (4*k+1,4*k+3), k>0.
 * https://oeis.org/A071701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{p ≤ n | p prime, p ≡ 1 (mod 4), p+2 prime}
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache of computed primes up to the current maximum limit. */
    static idx_t max_limit = 0;               /* maximum index for which we have marked primes */
    static unsigned char *prime = NULL;       /* prime flags: 0 → composite, 1 → prime */

    /* If n+2 is larger than our current sieve, extend it. */
    idx_t need = n + 2;
    if (need > max_limit) {
        unsigned char *new_prime = (unsigned char*)malloc((need + 1) * sizeof(unsigned char));
        if (!new_prime) return (seq_t)0;  /* allocation failed: return 0 */

        /* Initially mark every number as prime. */
        for (idx_t i = 0; i <= need; ++i) new_prime[i] = 1;
        new_prime[0] = new_prime[1] = 0;

        /* Sieve of Eratosthenes up to 'need'. */
        for (idx_t p = 2; p * p <= need; ++p) {
            if (new_prime[p]) {
                for (idx_t j = p * p; j <= need; j += p)
                    new_prime[j] = 0;
            }
        }

        /* Replace old table, if any. */
        if (prime) free(prime);
        prime = new_prime;
        max_limit = need;
    }

    /* Count twin prime pairs (p, p+2) with p ≤ n, p ≡ 1 (mod 4). */
    seq_t count = 0;
    if (n >= 5) {
        for (idx_t p = 5; p <= n; p += 4) {   /* numbers 1 mod 4 start at 5, step 4 */
            if (prime[p] && prime[p + 2]) {
                ++count;
            }
        }
    }
    return (seq_t)count;
}
