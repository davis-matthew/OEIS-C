/*
 * A089614  Number of primes between n^2 and (n+1/4)^2.
 * https://oeis.org/A089614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of primes p such that n^2 < p ≤ ⌊(n+1/4)^2⌋
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* count primes in the open–closed interval (n² , floor((n+1/4)²)]   */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

static unsigned char *sieve_bits = NULL;   /* bit i == 1 → i is composite  */
static idx_t sieve_limit = 0;              /* largest number that has been sieved */

/* test if a number is known to be composite */
static inline bool is_composite(idx_t n)
{
    return (sieve_bits[n >> 3] >> (n & 7)) & 1U;
}

/* mark a number as composite */
static inline void set_composite(idx_t n)
{
    sieve_bits[n >> 3] |= 1U << (n & 7);
}

/* enlarge the sieve so that all numbers up to 'limit' are marked */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit)   /* nothing to do */
        return;

    /* new byte count (at least 1 byte is always allocated) */
    size_t new_bytes = (limit >> 3) + 1;

    if (!sieve_bits) {
        sieve_bits = (unsigned char *)malloc(new_bytes);
        if (!sieve_bits) abort();
        memset(sieve_bits, 0, new_bytes);
    } else if (new_bytes > ((sieve_limit >> 3) + 1)) {
        size_t old_bytes = (sieve_limit >> 3) + 1;
        unsigned char *tmp = (unsigned char *)realloc(sieve_bits, new_bytes);
        if (!tmp) abort();
        sieve_bits = tmp;
        memset(sieve_bits + old_bytes, 0, new_bytes - old_bytes);
    }

    /* simple Eratosthenes extension from (sieve_limit+1) to limit */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_composite(p))
            continue;

        idx_t start = p * p;
        if (start <= sieve_limit) {
            start = (sieve_limit + 1 + p - 1) / p * p;
            if (start < p * p)
                start = p * p;
        }

        for (idx_t j = start; j <= limit; j += p)
            set_composite(j);
    }

    sieve_limit = limit;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    /* upper bound: floor((n + 1/4)^2) = floor(n² + n / 2) */
    __int128 n128 = (unsigned long long)n;
    __int128 upper128 = n128 * n128 + (n128 >> 1);  /* n/2 truncated */
    idx_t upper = (idx_t)upper128;

    /* lower bound: first integer > n² */
    __int128 low128 = n128 * n128 + 1;
    if (low128 > upper128)        /* empty interval */
        return 0;
    idx_t low = (idx_t)low128;

    /* ensure the sieve covers the interval */
    ensure_sieve(upper);

    /* count primes in [low, upper] (lower bound exclusive) */
    seq_t count = 0;
    for (idx_t p = low; p <= upper; ++p) {
        if (!is_composite(p))
            ++count;
    }
    return count;
}
