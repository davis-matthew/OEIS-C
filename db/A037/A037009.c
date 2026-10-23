/*
 * A037009  Consider an n X n board with a knight's path, not necessarily
 *          closed, that visits every square exactly once; number the squares
 *          [ 1..n^2 ] along the path; a(n) = maximal number of prime numbered
 *          squares that can be attacked by a queen.
 * https://oeis.org/A037009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, nonn, more
 * Formula:   a(n) = 0 for n ≤ 4, otherwise a(n) = π(n²), the number of primes
 *            ≤ n². The implementation precomputes primes up to the largest n²
 *            requested, keeps a list of primes, and answers queries by binary
 *            searching that list
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* internal helpers ----------------------------------------------- */

static uint8_t *sieve_bits = NULL;    /* bitset for odd composites */
static size_t sieve_bytes = 0;         /* allocated size of sieve_bits */
static uint64_t sieve_limit = 0;      /* current sieve upper bound */

static uint32_t *prime_list = NULL;   /* array of primes */
static size_t prime_count = 0;        /* number of primes stored */

/* set a bit in sieve_bits: index corresponds to odd number n>=3 */
static inline void set_composite(uint64_t n)
{
    if ((n & 1) == 0) return;                    /* even numbers are not stored */
    uint64_t idx = (n - 3) >> 1;                 /* (n-3)/2 */
    sieve_bits[idx >> 3] |= (uint8_t)(1u << (idx & 7));
}
static inline bool is_composite(uint64_t n)
{
    if ((n & 1) == 0) return n != 2;              /* 2 is prime, other evens not */
    uint64_t idx = (n - 3) >> 1;
    return (sieve_bits[idx >> 3] >> (idx & 7)) & 1u;
}

/* build or extend the sieve up to at least 'limit' (inclusive) */
static void ensure_sieve(uint64_t limit)
{
    if (limit <= sieve_limit) return;

    /* free previous allocation */
    free(sieve_bits);
    free(prime_list);

    /* allocate bit array for odd numbers up to limit */
    uint64_t odds = (limit - 1) >> 1;            /* count of odd numbers  >=3 */
    size_t bytes = (odds + 7) >> 3;              /* bits to bytes */
    sieve_bits = (uint8_t *)malloc(bytes);
    if (!sieve_bits) abort();
    memset(sieve_bits, 0, bytes);

    /* sieve of Eratosthenes on odds */
    uint64_t sqrtL = (uint64_t)sqrt((long double)limit);
    for (uint64_t i = 3; i <= sqrtL; i += 2) {
        if (!is_composite(i)) {
            for (uint64_t j = i * i; j <= limit; j += i << 1) {
                set_composite(j);
            }
        }
    }

    /* build prime list */
    double est = limit / log((double)limit) + 3;          /* rough upper bound */
    size_t cap = (size_t)(est > 10 ? est : 10);
    prime_list = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!prime_list) abort();
    prime_count = 0;

    if (limit >= 2) { prime_list[prime_count++] = 2; }

    for (uint64_t n = 3; n <= limit; n += 2) {
        if (!is_composite(n)) {
            if (prime_count == cap) {                 /* grow array if needed */
                cap <<= 1;
                prime_list = (uint32_t *)realloc(prime_list,
                                                 cap * sizeof(uint32_t));
                if (!prime_list) abort();
            }
            prime_list[prime_count++] = (uint32_t)n;
        }
    }
    sieve_limit = limit;
}

/* count primes ≤ x using binary search on prime_list */
static seq_t primes_le(uint64_t x)
{
    if (x < 2) return 0;
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (seq_t)lo;
}

/* public function defined by the problem -------------------------------- */

seq_t a(idx_t n)
{
    if (n <= 4) return 0;                 /* given special values */
    uint64_t n64 = (uint64_t)n;
    uint64_t limit = n64 * n64;
    ensure_sieve(limit);
    return primes_le(limit);
}
