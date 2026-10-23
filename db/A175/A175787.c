/*
 * A175787  Primes together with 4.
 * https://oeis.org/A175787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=2, a(2)=3, a(3)=4, a(n)=prime_{n-1} for n≥4
 * Range:     u8 n <= 55, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)4;

    /* static prime cache: primes[0]=2, primes[1]=3 */
    static seq_t *primes = NULL;
    static idx_t prime_count = 0;
    static idx_t capacity   = 0;
    static seq_t next_candidate = 5;  /* next odd to test */

    /* initialise cache on first call */
    if (primes == NULL) {
        capacity   = 16;
        primes     = (seq_t *)malloc((size_t)capacity * sizeof(seq_t));
        if (!primes) return (seq_t)0;           /* allocation failure */
        primes[0] = (seq_t)2;
        primes[1] = (seq_t)3;
        prime_count = 2;
    }

    /* generate primes until we have at least (n‑1) of them */
    while (prime_count < (n - 1)) {
        seq_t val = next_candidate;
        bool isPrime = true;

        /* trial division by primes up to sqrt(val) */
        for (idx_t i = 0; i < prime_count && primes[i] * primes[i] <= val; ++i) {
            if (val % primes[i] == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) {
            if (prime_count == capacity) {
                capacity *= 2;
                seq_t *tmp = (seq_t *)realloc(primes, (size_t)capacity * sizeof(seq_t));
                if (!tmp) return (seq_t)0; /* allocation failure */
                primes = tmp;
            }
            primes[prime_count++] = val;
        }
        next_candidate += 2; /* test only odd numbers */
    }

    /* prime_{n-1} is stored at index n-2 */
    return primes[n - 2];
}
