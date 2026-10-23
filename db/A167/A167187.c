/*
 * A167187  Primes obtained from other primes by prefixing a 7.
 * https://oeis.org/A167187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, digits, ensure_capacity, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A167187: Primes obtained by prefixing a 7 to a prime.
 * a(n) is the n‑th prime of the form 7·10^d(p)+p,
 * where p is a prime and d(p) is the number of decimal digits of p.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Utility function headers */
#include "../utils/build_primes_upto.h"
#include "../utils/digits.h"
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

/* ----- static storage for memoisation ----- */
static seq_t *terms      = NULL;  /* array of sequence values   */
static size_t terms_cap  = 0;     /* allocated capacity         */
static size_t terms_cnt  = 0;     /* number of filled terms     */

static uint64_t *primes      = NULL;  /* array of primes up to limit   */
static size_t primes_cnt     = 0;     /* number of primes in array     */
static idx_t   prime_limit   = 0;     /* current upper bound for primes*/
static size_t prime_idx_checked = 0;  /* index of next prime to test    */

/* Expand the prime list up to (and including) new_limit. */
static void extend_primes(idx_t new_limit)
{
    if (new_limit <= prime_limit)
        return;

    /* free old prime array if it exists */
    if (primes != NULL)
        free(primes);

    build_primes_upto(new_limit, &primes, &primes_cnt);

    prime_limit = new_limit;
}

/* Ensure we can store at least need elements in terms array. */
static void ensure_terms_capacity(size_t need)
{
    ensure_capacity(need, &terms_cap, (void **)&terms, sizeof(seq_t));
}

/* Generate terms up to a(n).  (All indices are 1‑based.) */
static void compute_terms_up_to(idx_t n)
{
    /* Start with a moderate upper bound for primes */
    if (prime_limit == 0)
        extend_primes(200000);

    while (terms_cnt < n) {
        /* Produce more primes if we have reached the end of the list. */
        while (prime_idx_checked >= primes_cnt) {
            extend_primes(prime_limit * 2 + 1);   /* double the limit */
            prime_idx_checked = 0;               /* restart scanning  */
        }

        /* Test the next prime p. */
        uint64_t p = primes[prime_idx_checked++];
        idx_t d  = digits((idx_t)p);          /* number of decimal digits */
        seq_t pow10d  = pow10((idx_t)d);      /* 10^d as seq_t          */
        seq_t candidate = (seq_t)7 * pow10d + (seq_t)p;  /* 7‖p */

        if (is_prime_big(candidate)) {
            ensure_terms_capacity(terms_cnt + 1);
            terms[terms_cnt++] = candidate;   /* store the prime candidate */
        }
    }
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)                       /* undefined, but safe for 0 */
        return (seq_t)0;

    compute_terms_up_to(n);
    return terms[n - 1];
}
