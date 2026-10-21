/*
 * A089101  a(n) = (n - 4 + prime(n) mod 9) mod 10.
 * https://oeis.org/A089101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n - 4 + prime(n) mod 9) mod 10
 * Range:     u8 n <= 54, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static storage for the list of primes */
static seq_t *primes_list = NULL;    /* array of found primes, 1‑based indexing */
static idx_t  primes_count = 0;      /* number of primes stored */
static idx_t  sieve_limit  = 0;      /* current upper bound of the sieve */

/* expand the sieve until it contains at least n primes */
static void
ensure_primes(idx_t n)
{
    if (primes_count >= n) return;             /* already enough */

    /* start with a small limit if this is the first call */
    if (sieve_limit == 0) sieve_limit = 20;

    /* keep enlarging until we have at least n primes */
    while (primes_count < n) {
        size_t new_limit = sieve_limit * 2;

        /* compute primes up to new_limit using a simple sieve */
        size_t  lim  = new_limit;
        size_t  i    = 0;
        size_t  primes_needed = lim / 10 + 10;  /* rough upper bound */

        /* temporary buffer for is_composite flags */
        unsigned char *is_comp = (unsigned char*)malloc(lim + 1);
        if (!is_comp) abort();
        for (i = 0; i <= lim; ++i) is_comp[i] = 0;

        for (size_t p = 2; (p * p) <= lim; ++p) {
            if (!is_comp[p]) {
                for (size_t m = p * p; m <= lim; m += p)
                    is_comp[m] = 1;
            }
        }

        /* reallocate primes_list to hold the new primes */
        seq_t *new_list = (seq_t*)realloc(primes_list,
                                          (sieve_limit + primes_needed) * sizeof(seq_t));
        if (!new_list) abort();
        primes_list = new_list;

        /* collect primes into primes_list */
        primes_count = 0;
        for (size_t p = 2; p <= lim; ++p) {
            if (!is_comp[p])
                primes_list[primes_count++] = (seq_t)p;
        }

        free(is_comp);
        sieve_limit = new_limit;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    ensure_primes(n);              /* guarantee that prime(n) is available */

    seq_t prime_n = primes_list[n - 1];   /* primes are 0‑based */
    /* compute result using signed arithmetic to handle negative values */
    int64_t tmp = (int64_t)n - 4 + (int64_t)(prime_n % 9);
    int64_t res = tmp % 10;
    if (res < 0) res += 10;        /* ensure non‑negative result */

    return (seq_t)res;
}
