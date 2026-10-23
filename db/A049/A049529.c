/*
 * A049529  Numbers k such that sum of factorials of digits of k equals pi(k)
 *          (A000720).
 * https://oeis.org/A049529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A049529: Numbers k such that the sum of factorials of the digits of k
   equals π(k)   (the number of primes <= k) */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Precomputed factorials of digits 0–9. */
static const uint32_t digit_fact[10] = {
    1,      /* 0! */
    1,      /* 1! */
    2,      /* 2! */
    6,      /* 3! */
    24,     /* 4! */
    120,    /* 5! */
    720,    /* 6! */
    5040,   /* 7! */
    40320,  /* 8! */
    362880  /* 9! */
};

/* Sum of factorials of the decimal digits of a number. */
static inline seq_t digit_fact_sum(idx_t x)
{
    seq_t sum = 0;
    if (x == 0) return (seq_t)digit_fact[0];
    while (x) {
        unsigned d = (unsigned)(x % 10);
        sum += (seq_t)digit_fact[d];
        x /= 10;
    }
    return sum;
}

/* Ensure the sieve covers all integers up to needed_limit.
   The sieve array is stored as 0 for prime, 1 for composite. */
static void ensure_sieve(idx_t needed_limit, uint8_t **sieve, idx_t *limit)
{
    if (*limit >= needed_limit) return;

    idx_t new_limit = *limit;
    if (new_limit == 0) new_limit = 1;
    while (new_limit < needed_limit) new_limit <<= 1;

    *sieve = (uint8_t *)realloc(*sieve, (size_t)new_limit + 1);
    if (!*sieve) exit(1);                /* memory failure */
    /* Zero the new part (old part already initialized). */
    memset(*sieve + (size_t)(*limit + 1), 0, (size_t)(new_limit - *limit));

    /* Mark 0 and 1 as composite. */
    (*sieve)[0] = 1;
    (*sieve)[1] = 1;

    /* Eratosthenes sieve up to new_limit. */
    size_t sqrt_lim = (size_t)sqrt((double)new_limit);
    for (size_t i = 2; i <= sqrt_lim; ++i) {
        if (!(*sieve)[i]) {
            for (size_t j = i * i; j <= (size_t)new_limit; j += i)
                (*sieve)[j] = 1;
        }
    }

    *limit = new_limit;
}

/* ------------------------------------------------------------------
   Main sequence function.  Static state is kept so calls can arrive in
   any order.   Returns the nth term (n >= 1). --------------------- */
seq_t a(idx_t n)
{
    /* Static cache of found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Sieve data. */
    static uint8_t *sieve = NULL;
    static idx_t sieve_limit = 0;

    /* Current position in the natural numbers. */
    static idx_t next_k = 1;
    static idx_t prime_count = 0;           /* number of primes <= next_k-1 */

    /* Initialise static data once. */
    if (!cache) {
        cache_cap = 256;
        cache = (seq_t *)calloc((size_t)cache_cap, sizeof(seq_t));
        if (!cache) return 0;               /* memory failure */

        sieve_limit = 100000;               /* initial size for sieve */
        sieve = (uint8_t *)calloc((size_t)(sieve_limit + 1), 1);
        if (!sieve) return 0;
        sieve[0] = sieve[1] = 1;
        size_t sqrt_lim = (size_t)sqrt((double)sieve_limit);
        for (size_t i = 2; i <= sqrt_lim; ++i)
            if (!sieve[i])
                for (size_t j = i * i; j <= (size_t)sieve_limit; j += i)
                    sieve[j] = 1;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Expand sieve if necessary. */
        if (next_k > sieve_limit)
            ensure_sieve(next_k + 1, &sieve, &sieve_limit);

        /* Update prime counter for current k. */
        if (next_k >= 2 && !sieve[next_k])
            ++prime_count;

        /* Check the condition for k = next_k. */
        seq_t sum_fact = digit_fact_sum(next_k);
        if (sum_fact == (seq_t)prime_count) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return 0;          /* memory failure */
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
