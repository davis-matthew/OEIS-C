/*
 * A179896  Sum of the numbers between k := n-th nonprime and 2k (like a jump
 *          in a Sieve of Eratosthenes).
 * https://oeis.org/A179896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*A045943(n)*(A045943(n)-1)/2 where A045943(n) is the nth
 *            nonprime (1,4,6,8,9,10,…)
 * Range:     u8 n <= 7, u16 n <= 163, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Static state ----------------------------------------------------------- */
static idx_t sieve_limit = 0;               /* current bound of the sieve   */
static uint8_t *is_prime      = NULL;       /* 1 = prime, 0 = composite   */
static idx_t  *nonprimes     = NULL;        /* list of nonprimes (1 also)   */
static idx_t   nonprimes_size = 0;           /* number of stored entries     */

/* Frees the array of nonprimes (if any). ------------------------------- */
static void free_nonprimes(void)
{
    if (nonprimes != NULL) {
        free(nonprimes);
        nonprimes = NULL;
    }
}

/* Ensures that the sieve is computed at least up to 'limit'.           */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;  /* already sufficient */

    /* Allocate/resize the primality table. */
    is_prime = (uint8_t *)realloc(is_prime,
                (size_t)(limit + 1) * sizeof(uint8_t));
    if (!is_prime) exit(1);

    /* Initialise all entries as prime. */
    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    /* Classical Sieve of Eratosthenes. */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                is_prime[m] = 0;
        }
    }

    /* Rebuild the list of nonprimes (>1).  Include 1 explicitly. */
    free_nonprimes();
    nonprimes = (idx_t *)malloc(((size_t)(limit > 0 ? limit : 1) + 1) *
                                 sizeof(idx_t));
    if (!nonprimes) exit(1);
    nonprimes_size = 0;
    if (limit >= 1)               /* 1 is the first nonprime */
        nonprimes[nonprimes_size++] = 1;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_prime[i])
            nonprimes[nonprimes_size++] = i;
    }

    sieve_limit = limit;
}

/* Main sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;         /* undefined for n < 1; return 0 */

    /* Index of the desired nonprime (0‑based). */
    idx_t target_index = n - 1;

    /* Grow the sieve until we have enough nonprimes. */
    while (nonprimes_size <= target_index) {
        idx_t new_limit = sieve_limit * 2;
        if (new_limit < 2) new_limit = 2;
        ensure_sieve(new_limit);
    }

    idx_t k = nonprimes[target_index];   /* the k‑th nonprime          */
    seq_t ks = (seq_t)k;
    seq_t result = ks * (ks - 1) * (seq_t)3 / 2;
    return result;
}
