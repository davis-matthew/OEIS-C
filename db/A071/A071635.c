/*
 * A071635  Number of decompositions of 4*n+2 into sum of two primes of form
 *          4*k+1.
 * https://oeis.org/A071635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 4041, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of unordered pairs of primes ≡ 1 (mod 4) whose sum is 4 · n + 2.
 * The function builds a sieve of Eratosthenes up to the required limit.
 * For each required n it extends the sieve when necessary so that subsequent
 * calls reuse the prime table.  The sieve and the static data are kept in
 * dynamic memory; the function is safe for non‑monotonic calls.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static storage for the prime sieve */
static size_t      sieve_limit = 0;     /* current highest index sieved */
static uint8_t    *is_prime    = NULL;  /* 0 = composite, 1 = prime */

/* build or rebuild a sieve up to 'limit' inclusively.  The array is allocated
 * with malloc and free'd if it already exists. */
static void build_sieve(size_t limit)
{
    /* allocate new array */
    uint8_t *new_array = (uint8_t *)malloc((limit + 1U) * sizeof(uint8_t));
    if (!new_array)
        abort();  /* out of memory – terminate with a fatal error */

    /* initialize all entries to 1 (prime) first */
    for (size_t i = 0; i <= limit; ++i)
        new_array[i] = 1U;
    new_array[0] = 0U;            /* 0 is not prime */
    if (limit >= 1)
        new_array[1] = 0U;        /* 1 is not prime */

    /* sieve of Eratosthenes: mark composites */
    for (size_t p = 2; p <= limit / p; ++p) {
        if (new_array[p]) {
            for (size_t q = p * p; q <= limit; q += p)
                new_array[q] = 0U;
        }
    }

    /* replace old sieve with the new one */
    if (is_prime)
        free(is_prime);
    is_prime     = new_array;
    sieve_limit  = limit;
}

/* external (OEIS) function definition */
seq_t a(idx_t n)
{
    /* compute the target number S = 4*n + 2 */
    size_t target = (size_t)(4ULL * n + 2ULL);

    /* extend the sieve if necessary */
    if (!is_prime || target > sieve_limit)
        build_sieve(target);

    seq_t count = 0;
    size_t half = target / 2U;   /* we only need primes up to half of S */

    for (size_t p = 2; p <= half; ++p) {
        if (!is_prime[p])          continue;        /* p is not prime */
        if (p % 4U != 1U)          continue;        /* not ≡1 (mod 4)   */

        size_t q = target - p;
        if (q < 2U)                continue;
        if (!is_prime[q])          continue;        /* q is not prime   */
        if (q % 4U != 1U)          continue;        /* not ≡1 (mod 4)   */

        ++count;                    /* unordered pair {p,q} counted once */
    }

    return count;
}
