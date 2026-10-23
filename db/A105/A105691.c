/*
 * A105691  Primes p such that both 2p + 3 and 4p + 5 are primes.
 * https://oeis.org/A105691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   is_prime_seq, is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A105691
   Primes p such that both 2p + 3 and 4p + 5 are primes.
   The function searches for terms on demand, caching results.
   It works for any seq_t type (arbitrary‑precision or native integers). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_uint64.h"   /* bool is_prime_uint64(uint64_t)   */
#include "../utils/is_prime_seq.h"     /* bool is_prime_seq(seq_t)         */

#define SIEVE_LIMIT 200000000ULL     /* should cover many terms (≈30 000) */

/* -------------------------------------------------------------------------- */
/* integer square root: works for all uint64_t values without <math.h>        */
/* -------------------------------------------------------------------------- */
static inline uint64_t isqrt64(uint64_t x)
{
    uint64_t r = 0, bit = 1ULL << 62;
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* -------------------------------------------------------------------------- */
/* simple sieve for 64–bit primes (odd numbers only)                        */
/* -------------------------------------------------------------------------- */
static unsigned char *prime_bits = NULL;    /* bitset: 0‑prime, 1‑composite */
static uint64_t  prime_bits_limit = 0;
static bool      sieve_ready = false;

static void init_sieve(void)
{
    prime_bits_limit = SIEVE_LIMIT;
    uint64_t bits = (prime_bits_limit >= 3) ? ((prime_bits_limit - 3) / 2 + 1) : 0;
    size_t   bytes = (bits + 7) >> 3;
    prime_bits = (unsigned char *)malloc(bytes);
    if (prime_bits == NULL) abort();
    for (size_t i = 0; i < bytes; ++i) prime_bits[i] = 0;

    uint64_t sqrt_lim = isqrt64(prime_bits_limit) + 1;
    for (uint64_t i = 3; i <= sqrt_lim; i += 2) {
        uint64_t idx = (i - 3) >> 1;
        if (prime_bits[idx >> 3] & (1U << (idx & 7))) continue;     /* composite */
        for (uint64_t j = i * i; j <= prime_bits_limit; j += i << 1) {
            uint64_t jdx = (j - 3) >> 1;
            prime_bits[jdx >> 3] |= (1U << (jdx & 7));
        }
    }
    sieve_ready = true;
}

/* check primality up to SIEVE_LIMIT with the sieve, beyond that use helper */
static inline bool is_sieve_prime(uint64_t n)
{
    if (n == 2) return true;
    if (n < 2 || (n & 1) == 0) return false;
    if (!sieve_ready) init_sieve();
    if (n > prime_bits_limit) {
        return is_prime_uint64(n);                /* generic 64‑bit test */
    }
    uint64_t idx = (n - 3) >> 1;
    return !(prime_bits[idx >> 3] & (1U << (idx & 7)));
}

/* -------------------------------------------------------------------------- */
/* storage of sequence terms and helper parameters                            */
/* -------------------------------------------------------------------------- */
static seq_t *seq_arr  = NULL;
static size_t seq_cap  = 0;
static size_t seq_len  = 0;
static uint64_t next_candidate = 3;   /* next odd number after the first term */

/* ensure array capacity */
static void ensure_seq_cap(size_t min_cap)
{
    if (min_cap <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    if (seq_arr == NULL) abort();
    seq_cap = new_cap;
}

/* -------------------------------------------------------------------------- */
/* main sequence function                                                     */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, harmless */

    if (!sieve_ready) init_sieve();

    /* initialise with the first term */
    if (seq_len == 0) {
        ensure_seq_cap(128);
        seq_arr[0] = (seq_t)2;            /* a(1) = 2 */
        seq_len = 1;
    }

    /* search until we have n terms */
    uint64_t cand = next_candidate;
    while (seq_len < (size_t)n) {
        if (is_sieve_prime(cand)) {
            seq_t p_val = (seq_t)cand;
            seq_t a_val = p_val * 2 + (seq_t)3;
            seq_t b_val = p_val * 4 + (seq_t)5;
            if (is_prime_seq(a_val) && is_prime_seq(b_val)) {
                ensure_seq_cap(seq_len + 1);
                seq_arr[seq_len++] = p_val;
            }
        }
        cand += 2;   /* next odd candidate */
    }

    next_candidate = cand;              /* keep iterator for future calls */
    return seq_arr[n - 1];
}
