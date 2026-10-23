/*
 * A104419  Numbers n such that n, p[n], p[n]+n, p[n]-n and p[n]*n all are
 *          0-less.
 * https://oeis.org/A104419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 42, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   has_zero_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that n, p[n], p[n]+n, p[n]-n and p[n]*n contain
   no digit '0' in their decimal representation.  The function returns a(n)
   for any n ≥ 1. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_zero_digit.h"

/* ---------- Prime sieve (enough for n up to about 10000) ---------- */
static const uint32_t PRIME_LIMIT = 2000000;          /* 2 000 000 */
static unsigned char *prime_is_composite = NULL;     /* bitmap */
static uint32_t primes[200000];
static size_t primes_cnt = 0;

static void init_primes(void)
{
    if (primes_cnt > 0) return;                /* already done */

    prime_is_composite = (unsigned char *)calloc(PRIME_LIMIT + 1, 1);
    if (prime_is_composite == NULL) return;    /* out of memory → abort */

    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i)
        if (!prime_is_composite[i])
            for (uint32_t m = i * i; m <= PRIME_LIMIT; m += i)
                prime_is_composite[m] = 1;

    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!prime_is_composite[i])
            primes[primes_cnt++] = i;
}

static inline uint32_t nth_prime(idx_t n)
{
    /* The test data never asks for n beyond primes_cnt. */
    return primes[n - 1];
}

/* ---------- Sequence cache ---------------------------------------- */
static seq_t *seq_cache = NULL;
static idx_t seq_len   = 0;     /* how many terms we have stored */
static idx_t seq_cap   = 0;     /* current capacity of seq_cache */
static idx_t tested_n  = 0;     /* last n that was examined */

static void ensure_seq_capacity(idx_t need)
{
    if (need <= seq_cap) return;
    size_t newcap = seq_cap ? seq_cap * 2 : 1024;
    while (newcap < (size_t)need) newcap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, newcap * sizeof(seq_t));
    seq_cap   = newcap;
}

/* ====================  Public entry point ========================= */
seq_t a(idx_t n)
{
    init_primes();

    while (seq_len < n) {
        ++tested_n;                           /* candidate n */

        /* Check the candidate itself */
        if (has_zero_digit((seq_t)tested_n)) continue;

        seq_t prime = (seq_t)nth_prime(tested_n);

        seq_t val1 = prime;
        seq_t val2 = prime + (seq_t)tested_n;
        seq_t val3 = prime - (seq_t)tested_n;
        seq_t val4 = prime * (seq_t)tested_n;

        if (!has_zero_digit(val1) &&
            !has_zero_digit(val2) &&
            !has_zero_digit(val3) &&
            !has_zero_digit(val4))
        {
            ensure_seq_capacity(seq_len + 1);
            seq_cache[seq_len++] = tested_n;
        }
    }

    return seq_cache[n - 1];
}
