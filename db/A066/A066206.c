/*
 * A066206  a(n) = Product_{k=1..n} prime(2k), where prime(k) is the k-th
 *          prime.
 * https://oeis.org/A066206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=1..n} prime(2k) (product of the first n even‑indexed
 *            primes)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static storage for primes and prefix products.  All variables are
 * lazily initialised the first time a() is called. */
static size_t primes_count   = 0;
static size_t primes_cap     = 0;
static int    *primes        = NULL;   /* primes[i] contains the (i+1)-st prime */

static size_t prod_size      = 0;      /* highest n for which prod[n] is available */
static size_t prod_cap       = 0;
static seq_t  *prod          = NULL;   /* prod[n] = a(n), prod[0] = 1 */

static int    next_candidate = 3;       /* next odd number to test for primality */

/* ------------------------------------------------------------------ */
/* Basic primality test – trial division using the primes already found. */
static bool is_prime_candidate(int cand)
{
    /* Test divisibility by all known primes ≤ √cand. */
    for (size_t i = 0; i < primes_count; ++i) {
        int p = primes[i];
        long long psq = (long long)p * p;
        if (psq > cand)            return true;   /* no divisor found */
        if (cand % p == 0)         return false;  /* composite */
    }
    return true;   /* cand is prime */
}

/* Append the next prime to the primes table. */
static void generate_next_prime(void)
{
    /* Find next odd prime. */
    while (!is_prime_candidate(next_candidate))
        next_candidate += 2;

    /* Grow the primes array if required. */
    if (primes_count == primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        int *new_arr   = (int *)realloc(primes, new_cap * sizeof(int));
        if (!new_arr) abort();
        primes     = new_arr;
        primes_cap = new_cap;
    }

    primes[primes_count++] = next_candidate;
    next_candidate += 2;
}

/* Ensure that the primes array contains at least the first 'need' primes. */
static void ensure_primes(size_t need)
{
    if (!primes) {
        /* Allocate the first prime (2). */
        primes_cap = 16;
        primes     = (int *)malloc(primes_cap * sizeof(int));
        if (!primes) abort();
        primes[0]      = 2;
        primes_count   = 1;
        next_candidate = 3;
    }

    while (primes_count < need)
        generate_next_prime();
}

/* Ensure that the product array can hold up to index 'need'. */
static void ensure_products(size_t need)
{
    if (!prod) {
        prod_cap = 16;
        prod = (seq_t *)malloc(prod_cap * sizeof(seq_t));
        if (!prod) abort();
        prod[0] = (seq_t)1;   /* a(0) = 1, used for multiplication base */
        prod_size = 0;
    }

    if (prod_cap <= need) {
        size_t new_cap = prod_cap ? prod_cap * 2 : 16;
        while (new_cap <= need) new_cap <<= 1;
        seq_t *new_arr = (seq_t *)realloc(prod, new_cap * sizeof(seq_t));
        if (!new_arr) abort();
        prod     = new_arr;
        prod_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* Main function: compute a(n).  The result is memoised in 'prod'. */
seq_t a(idx_t n)
{
    if (n == 0) {                 /* not used by OEIS, but harmless */
        return (seq_t)1;
    }

    size_t usz = (size_t)n;       /* convert to size_t for array indices */

    /* Make sure we have the 2n-th prime available. */
    ensure_primes(2 * usz);

    /* Extend the product array if necessary. */
    ensure_products(usz);

    /* Compute any products that are still missing. */
    for (size_t m = prod_size + 1; m <= usz; ++m) {
        /* prime(2m) corresponds to primes[2m-1] (0‑based). */
        seq_t p = (seq_t)primes[2 * m - 1];
        prod[m] = prod[m - 1] * p;
    }
    prod_size = usz;

    return prod[usz];
}
