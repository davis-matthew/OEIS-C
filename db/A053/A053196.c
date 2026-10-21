/*
 * A053196  Cototients of even numbers.
 * https://oeis.org/A053196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * n - φ(2 * n)
 * Range:     u8 n <= 132, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static idx_t max_primes_limit = 1;      /* largest number for which primes are known  */
static idx_t *primes = NULL;           /* array of primes                                    */
static size_t primes_size = 0;         /* number of primes stored in primes array             */
static size_t primes_cap  = 0;         /* allocated capacity of primes array                 */

static void ensure_primes(idx_t limit)
{
    if (limit <= max_primes_limit) return;          /* already have enough primes             */

    /* Simple sieve up to 'limit' */
    idx_t s = limit + 1;
    bool *is_p = (bool *)malloc(s);
    if (!is_p) return; /* allocation failure – unlikely for small limits   */

    for (idx_t i = 0; i < s; ++i) is_p[i] = true;
    is_p[0] = is_p[1] = false;
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_p[p]) {
            for (idx_t j = p * p; j <= limit; j += p) is_p[j] = false;
        }
    }

    /* Count primes to know new size */
    size_t new_count = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_p[i]) ++new_count;

    /* Reallocate the prime array if necessary */
    if (primes_cap < new_count) {
        idx_t *tmp = (idx_t *)realloc(primes, new_count * sizeof(idx_t));
        if (!tmp) { free(is_p); return; } /* allocation failure */
        primes = tmp;
        primes_cap = new_count;
    }

    /* Store primes in array */
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_p[i]) primes[idx++] = i;

    primes_size = new_count;
    max_primes_limit = limit;
    free(is_p);
}

/* Compute φ(m) using the precomputed primes */
static seq_t phi_of(seq_t m)
{
    if (m == 0) return 0;
    seq_t result = m;
    seq_t rem     = m;

    for (size_t i = 0; i < primes_size; ++i) {
        idx_t p = primes[i];
        if ((seq_t)p * (seq_t)p > rem) break;
        if (rem % p == 0) {
            while (rem % p == 0) rem /= p;
            result = result / p * (p - 1);
        }
    }
    if (rem > 1) {                  /* remaining part is a prime > sqrt(original m) */
        result = result / rem * (rem - 1);
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset: the sequence starts at a(1) */

    idx_t limit = n * 2;          /* we need primes up to 2*n for φ(2n) */
    ensure_primes(limit);

    seq_t two_n = ((seq_t)n << 1);
    seq_t phi   = phi_of(two_n);
    return two_n - phi;
}
