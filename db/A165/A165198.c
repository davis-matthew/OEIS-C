/*
 * A165198  Primes from integers by taking the factorial of each digit and
 *          adding them up.
 * https://oeis.org/A165198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n‑th prime obtainable as the sum of factorials of
 *            the decimal digits of some positive integer. The sequence
 *            records the prime values in the order the corresponding integers
 *            are encountered (duplicates are included)
 * Range:     u8 n <= 14, u16 n <= 5, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   digit_factorial, digit_factorial_sum, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/digit_factorial.h"
#include "../utils/digit_factorial_sum.h"

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/* cache of primes found so far (duplicates allowed) */
static seq_t *prime_cache       = NULL;    /* array of seq_t */
static size_t prime_cache_size = 0;       /* # elements stored */
static size_t prime_cache_cap  = 0;       /* array capacity   */
static idx_t  next_int         = 1;       /* next integer to test */

/* ensure we can write at least 'need' elements into prime_cache */
static void
ensure_capacity(size_t need)
{
    if (prime_cache_cap >= need)
        return;
    size_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 128;
    if (new_cap < need) new_cap = need;

    /* cast needed for C++ */
    seq_t *tmp = (seq_t *)realloc(prime_cache,
                                  new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    prime_cache      = tmp;
    prime_cache_cap  = new_cap;
}

/* ------------------------------------------------------------ */
/* main sequence function */
seq_t
a(idx_t n)
{
    if (n < 1)                /* undefined for n < 1 – return 0 */
        return (seq_t)0;

    while (prime_cache_size < (size_t)n) {
        seq_t val = digit_factorial_sum(next_int);
        if (is_prime_big(val)) {
            ensure_capacity(prime_cache_size + 1);
            prime_cache[prime_cache_size++] = val;
        }
        ++next_int;
    }
    return prime_cache[n - 1];
}
