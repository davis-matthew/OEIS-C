/*
 * A102616  Nonprime numbers of order 3.
 * https://oeis.org/A102616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the 3rd iteration of the nonprime index function.
    a(1) = nth_nonprime(nth_nonprime(nth_nonprime(1)))       */
/*  a(n) = nth_nonprime(nth_nonprime(nth_nonprime(n)))      */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache for the nonprime number list. */
static bool            cache_ready   = false;
static seq_t          *nonprimes     = NULL;
static idx_t           nonprime_count = 0;

/* ------------------------------------------------------------------ */
/*  Initialise a tiny sieve and build the list of nonprime numbers.    */
/* ------------------------------------------------------------------ */
static void init(void)
{
    if (cache_ready) return;

    /* Limit large enough to contain at least the first 10 000 nonprime
       numbers and their two further images. */
    const idx_t LIMIT = 30000;

    /* Allocate an array marking primality. */
    bool *isprime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!isprime) return;               /* allocation failure – unlikely */

    for (idx_t i = 0; i <= LIMIT; ++i) isprime[i] = true;
    isprime[0] = isprime[1] = false;

    for (idx_t p = 2; p * p <= LIMIT; ++p) {
        if (isprime[p]) {
            for (idx_t q = p * p; q <= LIMIT; q += p) {
                isprime[q] = false;
            }
        }
    }

    /* Count nonprimes up to LIMIT. */
    idx_t cnt = 0;
    for (idx_t i = 1; i <= LIMIT; ++i) {
        if (i == 1 || !isprime[i]) ++cnt;
    }

    /* Allocate the array that will hold the nonprime values. */
    nonprimes = (seq_t *)malloc((size_t)cnt * sizeof(seq_t));
    if (!nonprimes) { free(isprime); return; }

    /* Fill the array. */
    idx_t pos = 0;
    for (idx_t i = 1; i <= LIMIT; ++i) {
        if (i == 1 || !isprime[i]) {
            nonprimes[pos++] = (seq_t)i;
        }
    }

    nonprime_count = cnt;
    cache_ready    = true;

    free(isprime);
}

/* ------------------------------------------------------------------ */
/*  Return the k‑th nonprime number (1‑based).                         */
/* ------------------------------------------------------------------ */
static seq_t nth_nonprime(idx_t k)
{
    init();
    /* k is guaranteed to be valid for the expected n. */
    return nonprimes[k - 1];
}

/* ------------------------------------------------------------------ */
/*  Public function required by the build.                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init();
    seq_t r1 = nth_nonprime(n);                 /* first application  */
    seq_t r2 = nth_nonprime((idx_t)r1);         /* second application */
    seq_t r3 = nth_nonprime((idx_t)r2);         /* third application  */
    return r3;
}
