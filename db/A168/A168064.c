/*
 * A168064  Numbers n such that exactly one of n+-1 and n+-2 is prime.
 * https://oeis.org/A168064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 146, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A168064 : Numbers n such that exactly one of n±1,n±2 is prime
   Computed by generating primes up to the required range and testing each
   integer until the desired index is reached. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static unsigned char *prime_table = NULL; /* 0 = prime, 1 = composite */
static size_t prime_limit = 0;
static seq_t *seq_cache = NULL;
static size_t seq_capacity = 0;
static size_t seq_count = 0;
static idx_t next_candidate = 0; /* next integer to test */

/* Ensure that prime_table covers all integers up to `limit` (inclusive). */
static void ensure_prime_table(size_t limit)
{
    if (limit <= prime_limit)
        return;

    free(prime_table);
    prime_table = (unsigned char *)calloc(limit + 1, 1);
    if (!prime_table)
        abort();

    /* Mark composites: prime_table[i] == 0 means prime for i >= 2. */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (!prime_table[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                prime_table[j] = 1;
        }
    }
    /* 0 and 1 are not prime */
    if (limit >= 0) prime_table[0] = 1;
    if (limit >= 1) prime_table[1] = 1;

    prime_limit = limit;
}

/* Returns 1 if n is prime, 0 otherwise. */
static inline int is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return 0;
    if (n > prime_limit)      /* should not happen with the logic below */
        return 0;
    return prime_table[n] == 0;
}

seq_t a(idx_t n)
{
    while (seq_count < n) {
        /* Make sure we can test n±2 for primality */
        if (next_candidate + 2 > prime_limit)
            ensure_prime_table((size_t)(next_candidate + 2));

        int cnt = 0;
        /* n-2 */
        if (next_candidate >= 2 && !prime_table[next_candidate - 2])
            ++cnt;
        /* n-1 */
        if (next_candidate >= 1 && !prime_table[next_candidate - 1])
            ++cnt;
        /* n+1 */
        if (!prime_table[next_candidate + 1])
            ++cnt;
        /* n+2 */
        if (!prime_table[next_candidate + 2])
            ++cnt;

        if (cnt == 1) {
            /* Store value in cache */
            if (seq_count == seq_capacity) {
                seq_capacity = seq_capacity ? seq_capacity * 2 : 128;
                seq_cache = (seq_t *)realloc(seq_cache, seq_capacity * sizeof(seq_t));
                if (!seq_cache)
                    abort();
            }
            seq_cache[seq_count++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return seq_cache[n - 1];
}
