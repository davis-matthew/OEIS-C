/*
 * A161187  Let S = A089237\{0} = union of primes and nonzero squares;
 *          sequence gives indices of squares.
 * https://oeis.org/A161187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = pi(n^2) + n, where pi(x) is the number of primes <= x.
 *            Sequence A161187 gives the indices of squares in the sorted list
 *            of all primes and non‑zero squares. For n ≥ 1, a(n) is the
 *            position at which n² occurs in that list. The algorithm keeps a
 *            global prime sieve up to the maximum n² request...
 * Range:     u8 n <= 36, u16 n <= 899, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Global data for the sieve and prime list. */
static bool   *prime_bits   = NULL;   /* bit array: true = prime */
static idx_t  *prime_list   = NULL;   /* sorted list of primes */
static idx_t   prime_count  = 0;      /* number of primes stored */
static idx_t   sieve_limit  = 0;      /* maximum number sieved so far */

/* Ensure a prime sieve exists for all numbers up to 'limit'.  The sieve
   is re‑computed from scratch if a larger limit is requested. */
static void
ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;                       /* already sufficient */

    /* Allocate a bit array for 0 … limit.  Use 'bool' so that the code
       compiles both as C and C++. */
    bool *bits = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!bits)
        return;                       /* Out of memory; caller will fail */

    for (idx_t i = 0; i <= limit; ++i)
        bits[i] = true;
    bits[0] = bits[1] = false;

    /* Classic Eratosthenes sieve */
    for (idx_t p = 2; (uint64_t)p * p <= limit; ++p)
    {
        if (!bits[p])
            continue;
        for (idx_t m = p * p; m <= limit; m += p)
            bits[m] = false;
    }

    /* Count primes to determine required array size */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (bits[i])
            ++cnt;

    /* Allocate array of primes */
    idx_t *primes = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!primes) {
        free(bits);
        return;                       /* Out of memory */
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (bits[i])
            primes[idx++] = i;

    /* Replace old data */
    free(prime_bits);
    free(prime_list);
    prime_bits  = bits;
    prime_list  = primes;
    prime_count = cnt;
    sieve_limit = limit;
}

/* Binary search to find the number of primes <= x. */
static idx_t
count_primes_le(idx_t x)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (prime_list[mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (idx_t)lo;                  /* lo = #primes <= x */
}

/* Main OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)                        /* offset is 1; return 0 for safety */
        return (seq_t)0;

    /* Compute n² (fits in idx_t for requested range). */
    idx_t n2 = n * n;

    /* Ensure primes up to n² are available. */
    ensure_sieve(n2);

    /* Count primes <= n² and compute a(n). */
    idx_t pi_val = count_primes_le(n2);
    seq_t result = (seq_t)pi_val + (seq_t)n;
    return result;
}
