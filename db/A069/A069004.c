/*
 * A069004  Number of times n^2 + s^2 is prime for positive integers s < n.
 * https://oeis.org/A069004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1884, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Search method: count primes of the form n^2 + s^2 with 1 ≤ s < n.
 * The values are small (≤ 2·10⁸ for n ≤ 10 000) so a simple
 * sieve of Eratosthenes up to 2·n² is used to test primality quickly.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* global state for the sieve */
static uint8_t *sieve_arr   = NULL;   /* bit array of odd composites */
static size_t  sieve_bytes = 0;       /* number of bytes allocated          */
static idx_t   sieve_limit = 0;       /* inclusive maximal number sieved    */

/* build a sieve of Eratosthenes up to 'limit' (limit ≥ 2) */
static void build_sieve(idx_t limit)
{
    size_t odd_bits = (limit >> 1) + 1;          /* include number 1 as an odd bit */
    sieve_bytes = (odd_bits + 7) >> 3;          /* bytes needed for the bits */

    sieve_arr = (uint8_t *)malloc(sieve_bytes);
    if (!sieve_arr) return;                    /* allocation failure → no prime test! */
    memset(sieve_arr, 0, sieve_bytes);

    size_t sqrt_limit = (size_t)sqrt((double)limit);
    for (size_t i = 3; i <= sqrt_limit; i += 2) {
        size_t idx_i = i >> 1;
        if (sieve_arr[idx_i >> 3] & (1 << (idx_i & 7))) continue;
        for (size_t j = i * i; j <= limit; j += i << 1) {
            size_t idx_j = j >> 1;
            sieve_arr[idx_j >> 3] |= (1 << (idx_j & 7));
        }
    }
    sieve_limit = limit;
}

/* ensures that the sieve covers at least 'limit' */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;
    if (sieve_arr) free(sieve_arr);
    build_sieve(limit);
}

/* test primality using the sieve */
static bool is_prime_sieve(idx_t x)
{
    if (x < 2) return false;
    if (x == 2) return true;
    if ((x & 1) == 0) return false;

    size_t idx = (size_t)(x >> 1);  /* safe: x ≤ sieve_limit ≤ 2·10⁸ */
    return (sieve_arr[idx >> 3] & (1 << (idx & 7))) == 0;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    /* sieve must cover the maximum number we will test:
       n^2 + s^2 < 2*n^2 for s < n   */
    idx_t max_limit = (idx_t)2 * n * n;
    ensure_sieve(max_limit);

    seq_t count = 0;
    /* choose s parity so that n^2 + s^2 is odd (necessary for a prime) */
    idx_t s_start = (n % 2 == 0) ? 1 : 2;
    for (idx_t s = s_start; s < n; s += 2) {
        uint64_t val = (uint64_t)n * n + (uint64_t)s * s;
        if (is_prime_sieve((idx_t)val))
            ++count;
    }
    return count;
}
