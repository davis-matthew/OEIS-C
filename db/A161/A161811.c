/*
 * A161811  Difference between nonprime(n+2) and nonprime(n).
 * https://oeis.org/A161811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nonprime(n+2) - nonprime(n) (nonprime(k) = k-th nonprime
 *            number)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define LIMIT 200000  /* enough to cover n up to about 10000 */

static seq_t *nonprimes = NULL;     /* array storing nonprime(k) for k>=1 (index 0 for n=1) */
static size_t nonprimes_len = 0;    /* number of elements currently stored */

/* Build the list of nonprime numbers up to LIMIT.
   We first sieve primes up to LIMIT, then collect the nonprime numbers. */
static void init_nonprimes(void)
{
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) return;                     /* allocation failure – give up */

    for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t p = 2; p * p <= LIMIT; ++p) {
        if (is_prime[p]) {
            for (size_t q = p * p; q <= LIMIT; q += p) {
                is_prime[q] = false;
            }
        }
    }

    /* Count nonprimes to allocate array */
    size_t count = 0;
    for (size_t i = 0; i <= LIMIT; ++i) {
        if (!is_prime[i]) ++count;
    }

    nonprimes = (seq_t *)malloc(count * sizeof(seq_t));
    if (!nonprimes) { free(is_prime); return; }   /* allocation failure – give up */
    nonprimes_len = count;

    /* Fill the nonprime array */
    size_t idx = 0;
    for (size_t i = 0; i <= LIMIT; ++i) {
        if (!is_prime[i]) {
            nonprimes[idx++] = (seq_t)i;
        }
    }

    free(is_prime);
}

seq_t a(idx_t n)
{
    /* Ensure the nonprime list is built. */
    if (!nonprimes) init_nonprimes();

    /* Indices for nonprime(n) and nonprime(n+2).
       nonprimes[0] == nonprime(1), so offset by (k-1). */
    idx_t i1 = n - 1;          /* nonprime(n)  */
    idx_t i2 = n + 1;          /* nonprime(n+2) */

    /* In normal expected usage i2 < nonprimes_len.
       If out of bounds, we could extend the sieve, but for the limits
       specified above this does not happen. */
    return nonprimes[i2] - nonprimes[i1];
}
