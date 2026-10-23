/*
 * A156749  For all numbers k(n) congruent to -1 or +1 (mod 4) starting with
 *          k(n) = {3,5,7,9,11,...}, a(k(n)) is incremented by the congruence
 *          (mod 4) if k(n) is prime and by 0 if k(n) is composite.
 * https://oeis.org/A156749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A066339(2*n+1) - A066490(2*n+1)
 * Range:     i8 n <= 9671, i16 n <= 9671, i32 n <= 9671, i64 n <= 9671,
 *            i128 n <= 9671, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static uint64_t current_limit = 0;
static unsigned char *is_prime = NULL;
static int *cum1 = NULL;
static int *cum3 = NULL;

static void prepare_sieve(uint64_t limit)
{
    if (limit <= current_limit)
        return;

    /* free old data */
    free(is_prime);
    free(cum1);
    free(cum3);

    /* allocate new arrays */
    is_prime = (unsigned char *)malloc((limit + 1) * sizeof(unsigned char));
    cum1    = (int *)malloc((limit + 1) * sizeof(int));
    cum3    = (int *)malloc((limit + 1) * sizeof(int));

    if (!is_prime || !cum1 || !cum3) {
        /* fatal error: memory allocation failed */
        exit(1);
    }

    /* initial values for 0 and 1 */
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        is_prime[i] = 1;

    /* simple sieve */
    uint64_t root = (uint64_t)sqrt((double)limit);
    for (uint64_t p = 2; p <= root; ++p) {
        if (is_prime[p]) {
            for (uint64_t m = p * p; m <= limit; m += p)
                is_prime[m] = 0;
        }
    }

    /* prefix sums of primes in each residue class */
    cum1[0] = cum3[0] = 0;
    for (uint64_t i = 1; i <= limit; ++i) {
        cum1[i] = cum1[i-1];
        cum3[i] = cum3[i-1];
        if (is_prime[i]) {
            if ((i & 3) == 1)
                ++cum1[i];
            else if ((i & 3) == 3)
                ++cum3[i];
        }
    }

    current_limit = limit;
}

seq_t a(idx_t n)
{
    /* compute M = 2*n + 1, using indexed type to avoid overflow */
    idx_t M = 2 * n + 1;

    /* ensure sieve and prefix sums up to M */
    prepare_sieve((uint64_t)M);

    int diff = cum1[M] - cum3[M];
    return (seq_t)diff;
}
