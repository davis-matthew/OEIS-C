/*
 * A062081  Group the even numbers as (2,4), (6,8,10,12), (14,16,18,20,22,24),
 *          (26,28,30,32,34,36,38,40), ... then a(n) = LCM of the n-th group.
 * https://oeis.org/A062081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * LCM of the n-th group of even numbers. The n-th group
 *            consists of 2n consecutive even numbers: {2+2(k-1)k ,
 *            2+2(k-1)k+2 , … , 2+2(k-1)k+2(2n-1)}
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/* static data for prime generation */
static uint32_t *primes = NULL;
static size_t primes_count = 0;
static size_t primes_capacity = 0;
static idx_t max_sieve_limit = 0;

/* build a prime sieve up to 'limit' (only odds are stored) */
static void build_primes_upto(idx_t limit)
{
    /* size of bit array for odd numbers up to limit */
    size_t bits_size = ((limit >> 1) >> 3) + 1;
    uint8_t *sieve = (uint8_t *)malloc(bits_size);
    memset(sieve, 0, bits_size);

    idx_t sqrt_lim = (idx_t)sqrt((double)limit);
    for (idx_t p = 3; p <= sqrt_lim; p += 2) {
        size_t idxp = p >> 1;
        if (sieve[idxp >> 3] & (1 << (idxp & 7))) continue;
        for (idx_t mult = p * p; mult <= limit; mult += p << 1) {
            size_t idxm = mult >> 1;
            sieve[idxm >> 3] |= 1 << (idxm & 7);
        }
    }

    /* estimate number of primes for initial allocation */
    size_t est = 0;
    if (limit >= 2) est = (size_t)(limit / (log((double)limit) + 1.0)) + 10;
    if (est < 1) est = 1;

    if (primes_capacity < est) {
        primes_capacity = est;
        primes = (uint32_t *)realloc(primes, primes_capacity * sizeof(uint32_t));
    }
    primes_count = 0;
    primes[primes_count++] = 2;                 /* prime 2 */

    for (idx_t num = 3; num <= limit; num += 2) {
        size_t idxn = num >> 1;
        if (!(sieve[idxn >> 3] & (1 << (idxn & 7)))) {
            if (primes_count == primes_capacity) {
                primes_capacity *= 2;
                primes = (uint32_t *)realloc(primes,
                                primes_capacity * sizeof(uint32_t));
            }
            primes[primes_count++] = (uint32_t)num;
        }
    }

    max_sieve_limit = limit;
    free(sieve);
}

/* ensure primes up to 'limit' are available */
static void ensure_primes(idx_t limit)
{
    if (limit <= max_sieve_limit) return;
    build_primes_upto(limit);
}

/* compute LCM of consecutive integers from 'start' to 'end' inclusive */
static seq_t lcm_consecutive(idx_t start, idx_t end)
{
    seq_t result = 1;
    for (size_t i = 0; i < primes_count && primes[i] <= end; ++i) {
        idx_t p = primes[i];
        idx_t pk = p;                      /* current power of p, <= end */
        while (pk <= end / p) pk *= p;     /* largest power of p <= end */

        /* reduce pk until at least one multiple lies in [start,end] */
        while ((start + pk - 1) / pk > end / pk)
            pk /= p;

        if (pk > 1)
            result *= (seq_t)pk;           /* multiply into result */
    }
    return result;
}

/* OEIS sequence A062081 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                                            /* unused */
    idx_t start = 1 + (n - 1) * n;                                    /* first odd in group */
    idx_t end   = start + 2 * n - 1;                                  /* last odd in group */
    ensure_primes(end);
    seq_t lcm = lcm_consecutive(start, end);
    return lcm * 2;                                                   /* all terms are even */
}
