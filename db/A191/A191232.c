/*
 * A191232  Concatenation of primes written in base 2 (A004676).
 * https://oeis.org/A191232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth binary digit of the infinite string obtained by
 *            concatenating the binary representations of the primes in
 *            increasing order
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   bitlen_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/bitlen_u32.h"

static uint32_t *primes = NULL;      /* stored primes */
static size_t primes_cnt  = 0;       /* number of primes stored */
static uint64_t *cum_bits = NULL;    /* cumulative bit counts up to each prime */

/* Test primality of candidate using the primes already found. */
static bool is_prime_u32(uint32_t candidate) {
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > candidate)
            break;
        if (candidate % p == 0)
            return false;
    }
    return true;
}

/* Initialise with the first prime (2). */
static void init_primes(void) {
    primes = (uint32_t *)malloc(sizeof(uint32_t));
    cum_bits = (uint64_t *)malloc(sizeof(uint64_t));
    if (!primes || !cum_bits) exit(1);
    primes[0] = 2;
    cum_bits[0] = bitlen_u32(2);   /* 1 bit */
    primes_cnt = 1;
}

/* Generate and store the next prime after the last stored one. */
static void add_next_prime(void) {
    uint32_t last = primes[primes_cnt - 1];
    uint32_t candidate = (last == 2) ? 3 : last + 2;

    while (!is_prime_u32(candidate))
        candidate += 2;

    /* resize arrays to hold one more element */
    uint32_t *new_primes = (uint32_t *)realloc(primes,
                        (primes_cnt + 1) * sizeof(uint32_t));
    uint64_t *new_cum   = (uint64_t *)realloc(cum_bits,
                        (primes_cnt + 1) * sizeof(uint64_t));
    if (!new_primes || !new_cum)
        exit(1);

    primes   = new_primes;
    cum_bits = new_cum;

    primes[primes_cnt] = candidate;
    uint64_t cumulative = cum_bits[primes_cnt - 1] + bitlen_u32(candidate);
    cum_bits[primes_cnt] = cumulative;

    primes_cnt++;
}

/* Locate the prime containing the nth bit.  Assumes n ≤ cum_bits[primes_cnt‑1]. */
static size_t find_prime_index(uint64_t n) {
    size_t lo = 0, hi = primes_cnt - 1;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (cum_bits[mid] >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Compute the nth term of A191232. */
seq_t a(idx_t n)
{
    if (primes_cnt == 0) init_primes();

    /* extend the tables until n lies within the current range */
    while (n > cum_bits[primes_cnt - 1])
        add_next_prime();

    size_t pidx = find_prime_index((uint64_t)n);
    uint64_t prev = (pidx == 0) ? 0 : cum_bits[pidx - 1];
    uint32_t offset = (uint32_t)(n - prev - 1);            /* 0‑based in the prime */
    uint32_t bits   = bitlen_u32(primes[pidx]);           /* number of bits of the prime */
    uint32_t shift  = bits - 1 - offset;                  /* position of the desired bit */
    uint32_t bit    = (primes[pidx] >> shift) & 1U;       /* extract the bit */

    return (seq_t)bit;
}
