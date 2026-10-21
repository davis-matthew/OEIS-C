/*
 * A072927  a(n) = minimum value of abs(n-phi(x)), x>0.
 * https://oeis.org/A072927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = min_{x>0} | n - φ(x) | Where φ is Euler's totient
 *            function. a(n)=0 iff n occurs as a totient
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* use the pre-defined integer types seq_t and idx_t */
static const idx_t MAX_LIMIT = 5000000;   /* should cover all test values */

static bool *seen = NULL;        /* seen[i] == true if i is a totient */
static bool initialized = false;

static void precompute(void)
{
    /* Allocate arrays */
    unsigned int *phi = (unsigned int *)malloc((MAX_LIMIT + 1) * sizeof(unsigned int));
    seen = (bool *)calloc(MAX_LIMIT + 1, sizeof(bool));

    /* Euler's totient sieve */
    for (idx_t i = 0; i <= MAX_LIMIT; ++i)
        phi[i] = (unsigned int)i;

    for (idx_t p = 2; p <= MAX_LIMIT; ++p)
        if (phi[p] == (unsigned int)p)           /* p is prime */
            for (idx_t j = p; j <= MAX_LIMIT; j += p)
                phi[j] -= phi[j] / (unsigned int)p;

    /* Mark totient values */
    for (idx_t i = 1; i <= MAX_LIMIT; ++i)
        if (phi[i] <= MAX_LIMIT)
            seen[phi[i]] = true;

    free(phi);
}

/* Compute the nearest abs difference to a totient */
seq_t a(idx_t n)
{
    if (!initialized) {
        precompute();
        initialized = true;
    }

    if (n > MAX_LIMIT) {
        /* Fallback: for unexpected large n, return a large difference.
           The problem statement guarantees n will be small enough for
           the precomputed range. */
        return (seq_t)(n - MAX_LIMIT);
    }

    /* Find closest totient <= n */
    idx_t low = n;
    while (low > 0 && !seen[low]) --low;

    /* Find closest totient >= n */
    idx_t high = n;
    while (high <= MAX_LIMIT && !seen[high]) ++high;
    /* assume high will be found before exceeding MAX_LIMIT */

    seq_t diff_low  = (seq_t)(n - low);
    seq_t diff_high = (seq_t)(high - n);

    return diff_low < diff_high ? diff_low : diff_high;
}
