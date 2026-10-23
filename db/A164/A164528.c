/*
 * A164528  a(1)=2. a(n) = the smallest multiple of {the largest proper
 *          divisor of n} that is > a(n-1).
 * https://oeis.org/A164528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164528
   a(1) = 2
   For n > 1:
        d = largest proper divisor of n ( = n / smallest prime factor of n )
        a(n) = the smallest multiple of d that is > a(n-1)
*/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* return smallest prime factor of n (for n >= 2) */
static uint64_t smallest_prime_factor(uint64_t n) {
    if (n % 2ULL == 0ULL) return 2ULL;
    for (uint64_t i = 3ULL; i * i <= n; i += 2ULL)
        if (n % i == 0ULL) return i;
    return n;                      /* n is prime */
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;    /* array of previously computed a values */
    static size_t sz = 0;          /* number of elements stored (>=0) */

    /* base case */
    if (n == 1) return (seq_t)2;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 1);
        if (cache == NULL) return (seq_t)0;   /* fallback for safety */
        cache[0] = (seq_t)2;
        sz = 1;
    }

    /* extend cache if necessary */
    if ((size_t)n > sz) {
        size_t new_sz = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_sz);
        if (tmp == NULL) return (seq_t)0;     /* fallback for safety */
        cache = tmp;

        /* compute missing terms up to n */
        for (size_t i = sz + 1; i <= (size_t)n; ++i) {
            uint64_t spf = smallest_prime_factor((uint64_t)i);
            seq_t d   = (seq_t)i / (seq_t)spf;        /* largest proper divisor */
            seq_t prev = cache[(size_t)(i - 2)];      /* a(i-1) */
            seq_t k    = prev / d + 1;                /* next multiple factor */
            cache[(size_t)(i - 1)] = k * d;           /* a(i) */
        }
        sz = new_sz;
    }

    return cache[(size_t)(n - 1)];
}
