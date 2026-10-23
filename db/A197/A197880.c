/*
 * A197880  Squarefree part of ((2n-1)!)^(2n-3).
 * https://oeis.org/A197880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A197880: squarefree part of ((2n-1)!)^(2n-3) */
#include <stdint.h>
#include <stdlib.h>

/* -------------------------------------------------------------------------- */
/*  Helper: generate all primes <= limit using a simple sieve.               */
/*  The prime list is cached in static arrays and grown as needed.           */
/* -------------------------------------------------------------------------- */
static uint32_t *primes = NULL;    /* array of primes */
static size_t   prime_cnt = 0;     /* number of primes stored */
static uint32_t max_prime_gen = 0; /* highest prime generated so far */

/* Allocate or extend the sieve up to 'limit'.  This function may be called
   multiple times with increasing limits; when 'limit' > max_prime_gen we
   recompute the sieve from scratch (the size is small enough for n <= 1e4). */
static void ensure_primes(uint32_t limit)
{
    if (limit <= max_prime_gen)
        return;

    size_t sz = (size_t)limit + 1;
    char *is_prime = (char *)malloc(sz);      /* cast needed for C++ */
    if (!is_prime)
        return;                               /* silently abort on OOM */

    for (size_t i = 0; i < sz; ++i)
        is_prime[i] = 1;
    if (sz > 0) is_prime[0] = 0;
    if (sz > 1) is_prime[1] = 0;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++cnt;

    uint32_t *new_primes = (uint32_t *)realloc(primes, cnt * sizeof(uint32_t));
    if (!new_primes) {
        free(is_prime);
        return; /* abort silently on OOM */
    }
    primes     = new_primes;
    prime_cnt  = cnt;
    max_prime_gen = limit;

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            primes[idx++] = (uint32_t)i;

    free(is_prime);
}

/* -------------------------------------------------------------------------- */
/*  Compute the squarefree part of (2n-1)!   (exponent parity is enough).    */
/* -------------------------------------------------------------------------- */
static seq_t term_for_n(idx_t n)
{
    /* 2n-1 fits in 32 bits for all practical n in this project */
    uint32_t limit = (uint32_t)((uint64_t)2 * (uint64_t)n - 1);
    ensure_primes(limit);

    seq_t prod = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if (p > limit)
            break;

        /* Compute parity of exponent of p in (limit)! */
        uint32_t t = limit / p;
        uint8_t parity = 0;
        while (t) {
            parity ^= (t & 1);
            t /= p;
        }
        if (parity)
            prod *= (seq_t)p;
    }
    return prod;
}

/* -------------------------------------------------------------------------- */
/*  Public entry point required by the framework.  a(1) = 1,                */
/*  a(2) = 6, a(3) = 30, ...                                            */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;        /* undefined, but keep safe */
    return term_for_n(n);
}
