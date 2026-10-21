/*
 * A076429  Sum of numbers that cannot be written as t*p(n) + u*p(n+1) for
 *          nonnegative integers t, u, where p(n) is the n-th prime.
 * https://oeis.org/A076429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p(n)-1)*(p(n+1)-1)*(2*p(n)*p(n+1)-p(n)-p(n+1)-1)/12
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Store primes as uint64_t – sufficient for the needed range.
   Conversion to seq_t happens automatically in the arithmetic. */
static uint64_t *primes64   = NULL;
static size_t     primes_len = 0;
static size_t     primes_cap = 0;

/* Ensure that at least 'need' primes have been generated. */
static void ensure_primes(idx_t need)
{
    if ((size_t)need <= primes_len)
        return;

    /* Grow the prime array as necessary. */
    size_t cap = primes_cap ? primes_cap : 16;
    while (cap < (size_t)need) cap <<= 1;
    if (cap > primes_cap) {
        uint64_t *tmp = (uint64_t *)realloc(primes64, cap * sizeof(uint64_t));
        if (!tmp) abort();                 /* allocations must succeed */
        primes64 = tmp;
        primes_cap = cap;
    }

    /* Find primes until we have the required amount. */
    uint64_t candidate = primes_len ? primes64[primes_len - 1] + 1 : 2;
    while (primes_len < (size_t)need) {
        bool prime = true;
        uint64_t limit = (uint64_t)(sqrt((long double)candidate));
        for (size_t i = 0; i < primes_len && primes64[i] <= limit; ++i) {
            if (candidate % primes64[i] == 0) {
                prime = false;
                break;
            }
        }
        if (prime) {
            primes64[primes_len++] = candidate;
        }
        ++candidate;
    }
}

/* Sequence A076429 */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1 – 0 is never queried */
        return (seq_t)0;

    ensure_primes(n + 1);

    seq_t p1 = (seq_t)primes64[n - 1];
    seq_t p2 = (seq_t)primes64[n];     /* (n+1)-th prime */

    seq_t term1 = (p1 - 1) * (p2 - 1);
    seq_t term2 = (2 * p1 * p2 - p1 - p2 - 1);
    seq_t numerator = term1 * term2;
    seq_t result = numerator / 12;

    return result;
}
