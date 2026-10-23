/*
 * A134376  Numbers whose sum of prime factors (counted with multiplicity) is
 *          not prime.
 * https://oeis.org/A134376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 155, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A134376: a(n) is the n-th positive integer whose sum of prime
 * factors (counted with multiplicity) is not prime.
 *
 * The function computes values on demand by scanning integers starting
 * at 1, followed by a trial division factorisation provided by
 * factor_u64.  The results are cached so that repeated calls to a(n)
 * are efficient, even if the arguments are not monotone.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* The FFT utilities used for factorisation are provided by the host build. */
#include "../utils/factor_u64.h"
#include "../utils/isPrimeGeneric.h"

static seq_t *cache = NULL;              /* cached sequence values         */
static size_t cap = 0;                   /* current allocation capacity    */
static size_t count = 0;                 /* number of cached values        */
static uint64_t last_checked = 1;        /* last integer we have examined */

/* Ensure cache has at least 'needed' slots.  Doubles capacity as
 * necessary.  Abort on allocation failure. */
static void ensure_cache_size(size_t needed)
{
    if (cap >= needed)
        return;
    size_t newcap = cap ? cap * 2 : 64;
    while (newcap < needed)
        newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cap = newcap;
}

/* Return the n-th term of A134376 (1‑based).  n >= 1. */
seq_t a(idx_t n)
{
    /* Initialise the first element (value 1). */
    if (count == 0) {
        ensure_cache_size(1);
        cache[0] = (seq_t)1;
        count = 1;
        last_checked = 1;
    }

    /* If we already have the requested term, just return it. */
    if ((idx_t)n <= count)
        return cache[n - 1];

    /* Extend the cache until we have at least n elements. */
    while (count < (size_t)n) {
        uint64_t m = ++last_checked;      /* next integer to examine */
        seq_t sum = 0;

        if (m != 1) {
            /* factor_u64 gives arrays of factors and exponents. */
            uint64_t primes_arr[64];
            uint64_t exps_arr[64];
            size_t pf_cnt = 0;
            factor_u64(m, primes_arr, exps_arr, &pf_cnt);

            for (size_t i = 0; i < pf_cnt; ++i) {
                sum += (seq_t)primes_arr[i] * (seq_t)exps_arr[i];
            }
        }   /* else sum stays 0 for m == 1 */

        /* Include m if the sum of its prime factors is not prime. */
        /* 0 and 1 are treated as not prime by isPrimeGeneric. */
        bool ok = (sum < 2) || !isPrimeGeneric(sum);
        if (ok) {
            ensure_cache_size(count + 1);
            cache[count++] = (seq_t)m;
        }
    }

    return cache[n - 1];
}
