/*
 * A062502  Number of prime divisors (with repetition) of the nonprimes
 *          (including 1).
 * https://oeis.org/A062502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Ω( A018252(n) ), i.e. the number of prime factors (with
 *            multiplicity) of the n-th nonprime number (including 1).
 *            Indexing starts at a(1)=0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t sieve_limit = 0;          /* maximum value up to which we've sieved */
static uint8_t *isPrime = NULL;       /* sieve bitmap (1 = prime, 0 = composite) */
static idx_t *primes = NULL;          /* array of primes in the sieve range */
static size_t primes_cnt = 0;          /* number of primes stored */

/* Recompute a sieve up to at least 'limit' (inclusive).  On entry, any previous sieve is
   discarded and a new one is built.  This function is called only when a larger range
   is required. */
static void recompute_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;

    /* free old data if any */
    if (isPrime) free(isPrime);
    if (primes) free(primes);

    /* allocate sieve array and initialize */
    isPrime = (uint8_t*)calloc((size_t)limit + 1, 1);
    if (!isPrime) abort();             /* out of memory */
    isPrime[0] = isPrime[1] = 0;
    for (idx_t i = 2; i <= limit; ++i) isPrime[i] = 1;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (isPrime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                isPrime[j] = 0;
        }
    }

    /* collect primes into array */
    primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (isPrime[i]) ++primes_cnt;

    primes = (idx_t*)malloc(primes_cnt * sizeof(idx_t));
    if (!primes) abort();

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (isPrime[i]) primes[idx++] = i;

    sieve_limit = limit;
}

/* Return the n-th nonprime number (unordered numbering starts at 1 with value 1). */
static idx_t nth_nonprime(idx_t n, idx_t *found)
{
    /* start with a reasonable upper bound; double if necessary */
    idx_t upper = n * 2 + 100;
    if (upper < 10) upper = 10;

    for (;;) {
        recompute_sieve(upper);

        idx_t count = 1;            /* 1 is counted as first nonprime */
        if (n == 1) { *found = 1; return 1; }

        for (idx_t i = 2; i <= upper; ++i) {
            if (!isPrime[i]) {
                ++count;
                if (count == n) { *found = i; return i; }
            }
        }

        /* not enough range – double and try again */
        upper *= 2;
    }
}

/* Main sequence function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined behaviour for n<1, but return 0 */

    idx_t x;
    nth_nonprime(n, &x);

    if (x <= 1) return 0;  /* Ω(1) = 0 */

    seq_t omega = 0;
    idx_t value = x;

    for (size_t i = 0; i < primes_cnt && (idx_t)primes[i] * (idx_t)primes[i] <= value; ++i) {
        idx_t p = primes[i];
        while (value % p == 0) {
            ++omega;
            value /= p;
        }
    }
    if (value > 1) ++omega;  /* remaining prime factor */

    return omega;
}
