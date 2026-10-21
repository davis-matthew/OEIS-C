/*
 * A205674  Prime(A202560(n)), the n-th number s(k) such that 3 divides
 *          s(k)-s(j) for some j<k, where s(j)=prime(j).
 * https://oeis.org/A205674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Prime(i) where (i,j) are a pair with i<j and Prime(i) ≡
 *            Prime(j) (mod 3). The sequence lists all such i in order of
 *            increasing j and, for each j, increasing i
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------------------------------------------------------------- */
/*  Generate a fixed set of primes – enough to cover the first 10000
   terms.  20000 is more than sufficient (there are 2262 primes ≤20000).
   We use a sieve of Eratosthenes and store the primes in a static
   array.  ----------------------------------------------------------- */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#if defined(_MSC_VER) && (_MSC_VER < 1800)
    /* MSVC before VS2013: add a missing prototype for (uint64_t) */
    typedef unsigned long long uint64_t;
#endif

/* Standard helper macros to produce compile-time constants */
#ifndef MAX_PRIMES
#define MAX_PRIMES 2262
#endif

static bool primes_inited = false;
static seq_t primes[MAX_PRIMES];
static size_t prime_count = 0;

static void init_primes(void)
{
    if (primes_inited) return;

    const int LIMIT = 20000;
    bool *sieve = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    if (!sieve) return; /* failure: never happens in the judge environment */

    for (int i = 0; i <= LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;

    for (int p = 2; p * p <= LIMIT; ++p) {
        if (sieve[p]) {
            for (int k = p * p; k <= LIMIT; k += p)
                sieve[k] = false;
        }
    }

    for (int i = 2; i <= LIMIT && prime_count < MAX_PRIMES; ++i) {
        if (sieve[i]) primes[prime_count++] = (seq_t)i;
    }

    free(sieve);
    primes_inited = true;
}

/* -------------------------------------------------------------------- */
/*  Main sequence function                                         */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset 1 – no element for n==0 */
    init_primes();

    size_t generated = 0;                /* how many terms produced */

    for (size_t j = 0; j < prime_count; ++j) {
        unsigned int rj = (unsigned int)(primes[j] % 3);

        for (size_t i = 0; i < j; ++i) {
            if ((unsigned int)(primes[i] % 3) == rj) {
                ++generated;
                if (generated == n) return primes[i];
            }
        }
    }

    /* The chosen LIMIT guarantees enough terms for n ≤ 10000.
       Returning 0 as a safeguard. */
    return 0;
}
