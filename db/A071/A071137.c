/*
 * A071137  Number of times 2n+p is prime, with p=1 or prime p < n.
 * https://oeis.org/A071137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = #{ p | p=1 or prime p<n and 2*n+p is prime }
 * Range:     u8 n <= 3989, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation builds a standard sieve of Eratosthenes up to a
/*  dynamically-determined limit (3*n+10) on each call.  The sieve array
/*  and a list of primes are stored in static state so that repeated
/*  calls reuse the data without recomputing the whole sieve.  The sieve
/*  size is always sufficient to test 2*n+p for the desired range of
/*  n.  The count is returned as seq_t; for all values fitting in
/*  seq_t the result is exact.  This method runs in O(n log log n) for
/*  each distinct argument, which is easily below the 2‑second limit for
/*  n up to about 10⁴.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static idx_t sieve_limit = 0;
static uint8_t *prime_flag   = NULL;   /* 0 = composite, 1 = prime */
static idx_t *prime_list     = NULL;   /* list of primes < sieve_limit */
static size_t prime_count    = 0;

/* Build or rebuild sieve and prime list up to 'limit' (inclusive) */
static void rebuild_sieve(idx_t limit)
{
    /* Allocate prime_flag array */
    if (prime_flag) free(prime_flag);
    prime_flag = (uint8_t*)malloc((size_t)(limit + 1));
    if (!prime_flag) exit(EXIT_FAILURE);
    memset(prime_flag, 1, (size_t)(limit + 1));
    prime_flag[0] = prime_flag[1] = 0;

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (prime_flag[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                prime_flag[j] = 0;
        }
    }

    /* Allocate prime list large enough (limit+1 is an upper bound) */
    if (prime_list) free(prime_list);
    prime_list = (idx_t*)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!prime_list) exit(EXIT_FAILURE);
    prime_count = 0;
    for (idx_t i = 2; i < limit; ++i) {
        if (prime_flag[i]) {
            prime_list[prime_count++] = i;
        }
    }

    sieve_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range */

    /* Ensure sieve covers numbers needed for candidates 2*n+p */
    idx_t needed = 3 * n + 10;
    if (needed > sieve_limit)
        rebuild_sieve(needed);

    seq_t count = 0;

    /* p = 1 case */
    if (prime_flag[2 * n + 1]) ++count;

    /* p = prime < n */
    for (size_t i = 0; i < prime_count && prime_list[i] < n; ++i) {
        idx_t candidate = 2 * n + prime_list[i];
        if (candidate <= sieve_limit && prime_flag[candidate])
            ++count;
    }

    return count;
}
