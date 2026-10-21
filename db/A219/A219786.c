/*
 * A219786  Least number such that there are n-1 composite numbers between n+1
 *          and a(n) (both inclusive).
 * https://oeis.org/A219786
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n): the least integer a such that there are n‑1 composite
   numbers among n+1 … a inclusive.  The definition uses the OEIS offset
   2 (a(2) is the first entry). */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple integer square root, works for size_t values. */
static size_t int_sqrt(size_t n)
{
    size_t r = 0;
    while ((r + 1) * (r + 1) <= n) ++r;
    return r;
}

/* Sieve state */
static unsigned char *prime_flags = NULL;   /* 0: composite, 1: prime */
static size_t sieve_limit = 0;              /* current maximum checked value */
static uint64_t *prime_list = NULL;         /* discovered primes          */
static size_t prime_count = 0;              /* number of primes          */

/* Extend the sieve to include all numbers up to new_limit (inclusive). */
static void sieve_extend(size_t new_limit)
{
    size_t i, j, p;

    if (new_limit <= sieve_limit) return;

    /* Resize prime flag array */
    unsigned char *tmp = (unsigned char *)realloc(prime_flags, new_limit + 1);
    if (!tmp) exit(EXIT_FAILURE);
    prime_flags = tmp;

    /* Mark new positions as prime initially */
    for (i = sieve_limit + 1; i <= new_limit; ++i) prime_flags[i] = 1;

    /* Ensure 0 and 1 are not primes */
    if (new_limit >= 0) prime_flags[0] = 0;
    if (new_limit >= 1) prime_flags[1] = 0;

    /* Mark composites for already known primes */
    size_t max_root = int_sqrt(new_limit);
    for (i = 0; i < prime_count; ++i) {
        p = prime_list[i];
        if (p > max_root) break;
        size_t start = ((sieve_limit + 1 + p - 1) / p) * p;
        for (j = start; j <= new_limit; j += p) prime_flags[j] = 0;
    }

    /* Handle newly discovered primes in the added range */
    for (i = sieve_limit + 1; i <= new_limit; ++i) {
        if (prime_flags[i]) {                 /* i is prime */
            if (i > 1) {                      /* skip 0,1 */
                /* Append to prime list */
                if (!prime_list) {
                    prime_list = (uint64_t *)malloc(16 * sizeof(uint64_t));
                    if (!prime_list) exit(EXIT_FAILURE);
                    prime_count = 0;
                } else if (prime_count % 16 == 0) {
                    uint64_t *tmp2 = (uint64_t *)realloc(
                        prime_list, (prime_count + 16) * sizeof(uint64_t));
                    if (!tmp2) exit(EXIT_FAILURE);
                    prime_list = tmp2;
                }
                prime_list[prime_count++] = i;
            }
            if (i <= max_root && i > 1) {
                for (j = i * i; j <= new_limit; j += i) prime_flags[j] = 0;
            }
        }
    }

    sieve_limit = new_limit;
}

/* Return true if x is composite (x > 1). */
static bool is_composite(uint64_t x)
{
    if (x <= 1) return false;           /* 0 and 1 are not composite */
    if (x > sieve_limit) sieve_extend((size_t)x);
    return !prime_flags[(size_t)x];
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                /* undefined for offset < 2 */

    uint64_t target = n - 1;            /* number of composites required */
    uint64_t count = 0;
    uint64_t m = n + 1;                 /* start checking from n+1 */

    while (1) {
        if (is_composite(m)) ++count;
        if (count == target) break;
        ++m;
        if (m > sieve_limit) sieve_extend((size_t)m);
    }
    return (seq_t)m;
}
