/*
 * A104103  a(n) = ceiling(sqrt(prime(n))).
 * https://oeis.org/A104103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(sqrt(prime(n)))
 * Range:     u8 n <= 54, u16 n <= 7349, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    /* store previously computed primes */
    static seq_t *primes = NULL;
    static idx_t primes_count = 0;
    static uint64_t last_checked = 2;  /* next candidate to test */

    /* grow the primes array in blocks of 64 */
    const size_t BLOCK = 64;

    /* generate primes until we have the nth */
    while (primes_count < n) {
        uint64_t cand = last_checked;
        /* find next prime */
        while (!is_prime(cand)) {
            cand++;
        }

        /* allocate / enlarge array */
        if (primes_count == 0) {
            primes = (seq_t *)malloc(BLOCK * sizeof(seq_t));
        } else if (primes_count % BLOCK == 0) {
            seq_t *tmp = (seq_t *)realloc(primes,
                                          (primes_count + BLOCK) * sizeof(seq_t));
            if (!tmp) return 0;  /* allocation failure */
            primes = tmp;
        }

        primes[primes_count] = (seq_t)cand;
        primes_count++;
        last_checked = cand + 1;
    }

    /* compute a(n) = ceil(sqrt(prime(n))) */
    seq_t p = primes[n - 1];
    seq_t s = isqrt(p);          /* floor(sqrt(p)) */
    return (s * s == p) ? s : s + 1;
}
