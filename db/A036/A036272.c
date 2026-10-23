/*
 * A036272  Absolute values of differences of absolute values of second
 *          differences of primes.
 * https://oeis.org/A036272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A036272 – Absolute values of differences of absolute values of second differences of primes.
   a(n) = | |p_{n+2} - 2p_{n+1} + p_n| - |p_{n+3} - 2p_{n+2} + p_{n+1}| |,
   where p_k denotes the k-th prime (p_0 = 2). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* static prime cache and sieve bitmap */
static uint64_t *prime_list   = NULL;   /* array of primes */
static size_t   prime_count  = 0;      /* number of primes cached */
static idx_t    sieve_limit  = 0;      /* upper bound of last sieve */
static uint8_t *sieve_bitmap = NULL;   /* bitmap of sieved numbers */
static size_t   bitmap_size  = 0;      /* size of bitmap in bytes */

/* simple sieve of Eratosthenes up to 'limit' (inclusive) */
static void sieve_up_to(idx_t limit)
{
    /* allocate or resize the bitmap */
    if (bitmap_size < (size_t)(limit + 1)) {
        free(sieve_bitmap);
        sieve_bitmap = (uint8_t*)malloc(limit + 1);
        bitmap_size  = limit + 1;
    }

    /* mark all numbers as potential primes */
    memset(sieve_bitmap, 1, bitmap_size);
    if (limit >= 0) sieve_bitmap[0] = 0;
    if (limit >= 1) sieve_bitmap[1] = 0;

    /* mark composites */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (sieve_bitmap[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                sieve_bitmap[j] = 0;
            }
        }
    }

    /* count primes to allocate the array */
    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (sieve_bitmap[i]) ++count;
    }

    /* build the prime list */
    uint64_t *new_list = (uint64_t*)malloc(count * sizeof(uint64_t));
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (sieve_bitmap[i]) new_list[pos++] = i;
    }

    free(prime_list);
    prime_list  = new_list;
    prime_count = count;
    sieve_limit = limit;
}

/* ensure at least 'n+4' primes are cached (for p_n .. p_{n+3}) */
static void ensure_primes(idx_t n)
{
    if (prime_count >= n + 4) return;

    /* a simple safe upper bound for the (n+4)-th prime:
       about 20*(n+4)+100, which is sufficient for n up to at least 10000. */
    idx_t limit = (n + 4) * 20 + 100;
    if (limit > sieve_limit) {
        sieve_up_to(limit);
    }
}

seq_t a(idx_t n)
{
    ensure_primes(n);

    /* compute second differences of primes */
    int64_t d0 = (int64_t)prime_list[n + 2]
               - 2LL * (int64_t)prime_list[n + 1]
               + (int64_t)prime_list[n];
    int64_t d1 = (int64_t)prime_list[n + 3]
               - 2LL * (int64_t)prime_list[n + 2]
               + (int64_t)prime_list[n + 1];

    uint64_t abs0 = d0 >= 0 ? (uint64_t)d0 : (uint64_t)(-d0);
    uint64_t abs1 = d1 >= 0 ? (uint64_t)d1 : (uint64_t)(-d1);
    uint64_t diff = abs0 > abs1 ? abs0 - abs1 : abs1 - abs0;

    return (seq_t)diff;
}
