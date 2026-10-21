/*
 * A088146  n-th prime rotated one binary place to the right.
 * https://oeis.org/A088146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = rotate_right_by_one_bit( prime_n )
 * (prime_1 = 2, prime_2 = 3, etc.)
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Dynamic cache of the first primes */
static seq_t *prime_cache      = NULL;   /* array of primes */
static idx_t   prime_count     = 0;      /* number of primes stored */
static idx_t   prime_capacity  = 0;      /* current array capacity */
static idx_t   next_candidate  = 2;      /* next integer to test for primality */

/* Increase the cache capacity if needed */
static void grow_cache(idx_t min_needed)
{
    if (prime_capacity >= min_needed)
        return;
    if (prime_capacity == 0)
        prime_capacity = 8;
    while (prime_capacity < min_needed)
        prime_capacity <<= 1;
    prime_cache = (seq_t *)realloc(prime_cache,
                                   (size_t)prime_capacity * sizeof(seq_t));
}

/* Simple trial‑division test for a candidate number,
 * using all stored primes up to sqrt(candidate).
 */
static bool is_prime_candidate(seq_t candidate)
{
    if (candidate < 2)
        return false;
    for (idx_t i = 0; i < prime_count; ++i) {
        seq_t p = prime_cache[i];
        if ((seq_t)p * (seq_t)p > candidate)
            break;
        if (candidate % p == 0)
            return false;
    }
    return true;
}

/* Ensure that prime_cache contains at least n primes. */
static void ensure_primes(idx_t n)
{
    if (prime_count >= n)
        return;

    grow_cache(n);

    while (prime_count < n) {
        if (is_prime_candidate(next_candidate))
            prime_cache[prime_count++] = next_candidate;
        ++next_candidate;
    }
}

/* Return the number of bits needed to represent v (v > 0). */
static unsigned int bitlen_seq_t(seq_t v)
{
    unsigned int len = 0;
    while (v != 0) {
        ++len;
        v >>= 1;
    }
    return len;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* 1‑based index: a(1) is the first prime (2) */
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, but we return 0 */

    ensure_primes(n);

    seq_t p = prime_cache[n - 1];          /* nth prime (1‑based) */
    unsigned int bits = bitlen_seq_t(p);   /* number of bits in p */
    seq_t low  = p & (seq_t)1;             /* least significant bit */
    seq_t rot  = (p >> 1) | (low << (bits - 1));  /* rotate right by 1 */

    return rot;
}
