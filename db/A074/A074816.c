/*
 * A074816  a(n) = 3^A001221(n) = 3^omega(n).
 * https://oeis.org/A074816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy, changed
 * Formula:   a(n) = 3^ω(n) where ω(n) is the number of distinct prime factors
 *            of n
 * Range:     u8 n <= 9949, u16 n <= 9949, u32 n <= 9949, u64 n <= 9949,
 *            u128 n <= 9949, big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow3.h"

/* static helper: smallest prime factor sieve for indexing up to the current maximum n */
static uint32_t *spf = NULL;      /* array of smallest prime factors; index 0..max_n */
static size_t max_n_alloc = 0;    /* size of allocated spf array */

/* sieving function to fill spf[2..N] */
static void sieve_spf(size_t N)
{
    /* allocate or reallocate spf array */
    uint32_t *new_spf = (uint32_t *)realloc(spf, (N + 1) * sizeof(uint32_t));
    if (!new_spf) return;                  /* allocation failure: leave spf NULL */

    spf = new_spf;
    max_n_alloc = N;

    /* initialize spf values to 0 */
    for (size_t i = 0; i <= N; ++i) spf[i] = 0;

    for (size_t i = 2; i <= N; ++i)
    {
        if (spf[i] == 0)            /* i is prime */
        {
            spf[i] = (uint32_t)i;
            /* avoid overflow in i*i */
            if (i > N / i) continue;
            for (size_t j = i * i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* sequence starts at n=1; value for n<1 is undefined */

    /* ensure we have spf info up to n */
    if ((size_t)n > max_n_alloc)
        sieve_spf((size_t)n);

    /* count distinct prime factors using spf */
    idx_t tmp = n;
    unsigned int distinct = 0;
    while (tmp > 1)
    {
        uint32_t p = spf[(size_t)tmp];
        ++distinct;
        do { tmp /= p; } while (tmp % p == 0);
    }

    /* a(n) = 3^distinct */
    return pow3((idx_t)distinct);
}
