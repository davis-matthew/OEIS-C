/*
 * A219185  Number of prime pairs {p,q} (p>q) with 3(p-q)-1 and 3(p-q)+1 both
 *          prime such that p+(1+(n mod 2))q=n.
 * https://oeis.org/A219185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Formula (conceptual): a(n) = number of unordered prime pairs {p,q} with
   p > q, 3(p-q)-1 and 3(p-q)+1 both prime,
   and p + (1 + (n mod 2))·q = n.  For even n this is p + q = n,
   for odd n it is p + 2q = n.
*/

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Static state for shared prime data */
static bool *prime_flags = NULL;
static size_t prime_limit = 0;
static idx_t *prime_list = NULL;  /* primes in ascending order */
static size_t prime_count = 0;

/* Ensure that prime_flags[0..limit] contains primality information
   and prime_list contains all primes up to limit.  */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= (idx_t)prime_limit)
        return;

    /* free old data */
    free(prime_flags);
    free(prime_list);

    prime_limit = (size_t)limit;
    prime_flags = (bool *)malloc((prime_limit + 1) * sizeof(bool));
    for (size_t i = 0; i <= prime_limit; ++i)
        prime_flags[i] = true;
    if (prime_limit >= 0) prime_flags[0] = false;
    if (prime_limit >= 1) prime_flags[1] = false;

    for (idx_t i = 2; i * i <= (idx_t)prime_limit; ++i) {
        if (prime_flags[i]) {
            for (idx_t j = i * i; j <= (idx_t)prime_limit; j += i)
                prime_flags[j] = false;
        }
    }

    /* Count primes for list allocation */
    size_t cnt = 0;
    for (idx_t i = 2; i <= (idx_t)prime_limit; ++i)
        if (prime_flags[i])
            ++cnt;

    prime_list = (idx_t *)malloc(cnt * sizeof(idx_t));
    size_t idx = 0;
    for (idx_t i = 2; i <= (idx_t)prime_limit; ++i)
        if (prime_flags[i])
            prime_list[idx++] = i;

    prime_count = cnt;
}

/* Helper: primality test using the built table or the generic routine. */
static inline bool table_or_generic(idx_t n)
{
    if (n <= (idx_t)prime_limit)
        return prime_flags[n];
    return is_prime((unsigned long long)n);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset starts at 1; safety */
        return (seq_t)0;

    /* Ensure we have primes up to the largest number that may appear: 3*n+1. */
    ensure_primes_up_to(3 * n + 1);

    size_t count = 0;
    bool is_even = ((n & 1u) == 0);

    /* Determine the maximum legal q for each parity of n. */
    idx_t limit_q = is_even ? (n - 1) / 2 : (n - 1) / 3;

    for (size_t i = 0; i < prime_count; ++i) {
        idx_t q = prime_list[i];
        if (q > limit_q)            /* primes are sorted */
            break;

        idx_t p;
        if (is_even) {
            p = n - q;               /* equation: p + q = n */
        } else {
            p = n - 2 * q;           /* equation: p + 2q = n */
        }

        if (p <= q || p < 2)         /* must be prime and > q */
            continue;

        if (!table_or_generic((idx_t)p))
            continue;

        idx_t d = p - q;
        idx_t val1 = 3 * d - 1;
        idx_t val2 = 3 * d + 1;

        if (!table_or_generic(val1))
            continue;
        if (!table_or_generic(val2))
            continue;

        ++count;
    }

    return (seq_t)count;
}
