/*
 * A103781  Sum of any four successive terms is prime, a(1)=a(2)=0,a(3)=1.
 * https://oeis.org/A103781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 153, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   estimate_prime_upper_bound, seq_malloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A103781
    Formula: a(n) = A000040(n-3) – a(n-1) – a(n-2) – a(n-3)
    Initial values: a(1)=0, a(2)=0, a(3)=1
    (uses the standard recurrence; all other terms are computed by it) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/seq_malloc.h"
#include "../utils/estimate_prime_upper_bound.h"

static seq_t *a_vals = NULL;   /* a(1) stored at a_vals[0] */
static idx_t  a_len  = 0;      /* number of computed a-values */

static seq_t *prime_vals = NULL;   /* prime(1) stored at prime_vals[0] */
static idx_t  prime_len  = 0;      /* number of primes stored */

/* Ensure that the first 'k' primes are available in prime_vals. */
static void
ensure_prime_up_to(idx_t k)
{
    if (prime_len >= k)
        return;    /* already have enough primes */

    /* Estimate an upper bound for the k‑th prime and generate all
       primes up to that bound using a simple sieve. */
    uint64_t limit = estimate_prime_upper_bound(k);
    unsigned char *is_comp = (unsigned char *)calloc((size_t)limit + 1, sizeof(unsigned char));
    if (!is_comp)
        return;     /* allocation failure, should not happen */

    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;
        }
    }

    /* Allocate space for k primes (we stop after k are found). */
    seq_t *new_primes = seq_malloc(k);
    idx_t  count = 0;
    for (uint64_t n = 2; n <= limit && count < k; ++n) {
        if (!is_comp[n]) {
            new_primes[count++] = (seq_t)n;   /* implicit conversion */
        }
    }
    free(is_comp);

    /* Replace the old prime array (if any). */
    if (prime_vals)
        free(prime_vals);
    prime_vals = new_primes;
    prime_len  = count;
}

/* Ensure that a_vals contains values up to (and including) index n. */
static void
ensure_a_capacity(idx_t n)
{
    if (a_len >= n)
        return;    /* already computed up to n */

    idx_t new_len = (a_len == 0) ? 4 : a_len * 2;
    if (new_len < n)
        new_len = n;

    seq_t *tmp = seq_malloc(new_len);
    if (!tmp)
        return;    /* allocation failure, should not happen */

    if (a_vals) {   /* copy existing values */
        for (idx_t i = 0; i < a_len; ++i)
            tmp[i] = a_vals[i];
        free(a_vals);
    }
    a_vals = tmp;

    /* Compute new a-values. */
    for (idx_t i = a_len + 1; i <= new_len; ++i) {
        seq_t val;
        if (i == 1)
            val = 0;
        else if (i == 2)
            val = 0;
        else if (i == 3)
            val = 1;
        else {
            /* a(n) = prime(n-3) – a(n-1) – a(n-2) – a(n-3)   (recurrence) */
            ensure_prime_up_to(i - 3);
            seq_t p = prime_vals[i - 4];          /* prime(i-3), 1‑based */
            val = p - a_vals[i - 2] - a_vals[i - 3] - a_vals[i - 4];
        }
        a_vals[i - 1] = val;
    }
    a_len = new_len;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n<1, return 0 */

    ensure_a_capacity(n);
    return a_vals[n - 1];
}
