/*
 * A200050  a(2) = 1, then (p-1)*(p-4)/2, with p = prime(n), n > 2.
 * https://oeis.org/A200050
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 72, u32 n <= 7108, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A200050
   Formula: a(2) = 1, for n>2: a(n) = (prime(n)-1)*(prime(n)-4)/2
   where prime(n) is the n-th prime (prime(1)=2).  The function provides
   all terms for n ≥ 2 and uses a simple incremental sieve to find
   primes on demand.  It is fully self‑contained and compiles under
   C11 and C++17. */

#include <stdint.h>
#include <stdlib.h>

/* Static storage for the list of primes encountered so far.  primes[i]
   holds prime(i+1).  primes_cap tracks the allocated size. */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

/* Grow the primes array to at least `new_cap` elements.  The growth
   factor is 2 for simplicity. */
static void primes_grow(size_t new_cap)
{
    if (new_cap <= primes_cap)
        return;
    size_t grow = primes_cap ? primes_cap * 2 : 16;
    if (grow < new_cap)
        grow = new_cap;
    uint64_t *new_arr = (uint64_t *)realloc(primes, grow * sizeof(uint64_t));
    /* In a failed compilation environment, it's acceptable for
       realloc to return NULL when memory is exhausted; such a failure
       will propagate as a NULL return from a(), which callers can
       detect. */
    if (new_arr == NULL)
        return;                 /* keep old array; caller will fail later */
    primes = new_arr;
    primes_cap = grow;
}

/* Append a new prime to the list. */
static void primes_append(uint64_t p)
{
    primes_grow(primes_cnt + 1);
    if (primes_cap < primes_cnt + 1)
        return;                 /* allocation failed; ignore */
    primes[primes_cnt++] = p;
}

/* Test if x is prime using the currently known primes (sufficient for
   incremental generation). */
static int is_prime_uint64(uint64_t x)
{
    if (x < 2)
        return 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > x)
            break;
        if (x % p == 0)
            return 0;
    }
    return 1;
}

/* Ensure that at least `n` primes are stored.  If more are needed,
   generate them one by one.  This is deliberately simple but
   sufficient for n up to ~10^5. */
static void ensure_primes(size_t n)
{
    if (primes_cnt >= n)
        return;
    uint64_t candidate = 2;
    /* If primes already populated, start from the next candidate after
       the last known prime. */
    if (primes_cnt > 0)
        candidate = primes[primes_cnt - 1] + 1;
    while (primes_cnt < n) {
        if (is_prime_uint64(candidate))
            primes_append(candidate);
        ++candidate;
    }
}

/* Sequence function.  It returns seq_t, which may be any unsigned
   integer type (8,16,32,64,128 bits or arbitrary precision).  All
   intermediate arithmetic is performed in seq_t, after converting
   the prime to that type.  For n < 2 the value is undefined; here we
   return 0. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;   /* undefined for the offset */
    if (n == 2)
        return (seq_t)1;   /* a(2) = 1 */

    ensure_primes((size_t)n);
    if (primes_cnt < (size_t)n)   /* allocation failure */
        return (seq_t)0;

    uint64_t p64 = primes[n - 1];        /* prime(n) */
    seq_t p = (seq_t)p64;                /* convert to seq_t */
    seq_t res = ((p - 1) * (p - 4)) / 2; /* formula */
    return res;
}
