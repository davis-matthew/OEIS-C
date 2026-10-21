/*
 * A228031  Primes of the form 7^k + 10.
 * https://oeis.org/A228031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   is_prime_big, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form 7^k + 10 (OEIS A228031) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility headers - provided by the build environment */
#include "../utils/mul_small.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    /* Static state to cache found primes and the current power of 7 */
    static seq_t *primes   = NULL;   /* array of primes of this form */
    static size_t  cap     = 0;      /* capacity of the array */
    static size_t  count   = 0;      /* number of primes stored so far */
    static seq_t  power    = 1;      /* 7^k, starting with k = 0 */

    /* Allocate initial cache */
    if (primes == NULL) {
        cap = 16;
        primes = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!primes) exit(EXIT_FAILURE);
    }

    /* Generate further primes until we have at least n of them */
    while (count < n) {
        seq_t val = power + (seq_t)10;          /* candidate 7^k + 10 */

        if (is_prime_big(val)) {               /* test primality */
            if (count == cap) {                /* grow cache if needed */
                cap <<= 1;
                seq_t *tmp = (seq_t*)realloc(primes, cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                primes = tmp;
            }
            primes[count++] = val;
        }

        power = mul_small(power, 7);            /* advance to next 7^k */
    }

    return primes[n - 1];
}
