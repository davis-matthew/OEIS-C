/*
 * A074627  Numbers n such that sigma(n) is divisible by 6.
 * https://oeis.org/A074627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th integer m such that sigma(m) ≡ 0 (mod 6)
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (σ(m) is the sum of divisors of m) */

#include <stdlib.h>
#include <stdint.h>

static uint8_t *sigma = NULL;   /* sigma modulo 6, indices 1 .. limit */
static size_t   sigma_limit = 0;/* current maximum index reached */

/* Rebuild the sigma array up to 'limit', computing sigma modulo 6 for all indices */
static void rebuild(size_t limit)
{
    /* free old array if it exists */
    if (sigma) free(sigma);

    /* allocate new array and initialise to 0 */
    sigma = (uint8_t*)calloc(limit + 1, sizeof(uint8_t));
    if (!sigma) abort(); /* allocation failure cannot be recovered */

    /* compute sigma modulo 6 by a simple sieve */
    for (size_t i = 1; i <= limit; ++i) {
        uint8_t imod6 = (uint8_t)(i % 6);
        for (size_t j = i; j <= limit; j += i) {
            uint8_t val = sigma[j] + imod6;
            if (val >= 6) val -= 6;          /* keep modulo 6 */
            sigma[j] = val;
        }
    }

    sigma_limit = limit;
}

/* Return the n-th term of the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, guard */
    size_t need = (size_t)n;

    /* initial guess for the upper bound */
    size_t limit = need * 10;
    if (limit < 100) limit = 100;

    while (1) {
        if (sigma_limit < limit) rebuild(limit);

        /* scan once to find the nth qualifying number */
        size_t count = 0;
        for (size_t i = 1; i <= limit; ++i) {
            if (sigma[i] == 0) {
                ++count;
                if (count == need) {
                    return (seq_t)i;
                }
            }
        }

        /* not enough terms yet, increase the limit and try again */
        limit *= 2;
    }
}
