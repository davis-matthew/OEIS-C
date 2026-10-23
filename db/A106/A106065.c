/*
 * A106065  Primes p such that 1*p+18 and 18*p+1 are primes.
 * https://oeis.org/A106065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   isPrime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A106065: primes p such that p+18 and 18p+1 are prime.
   The function performs an incremental search and caches found terms. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/isPrime64.h"

static seq_t *terms = NULL;      /* cached sequence values */
static size_t term_count = 0;    /* number of values stored */
static size_t term_cap = 0;      /* allocated capacity */
static uint64_t last_checked = 3;/* next integer to test (start from 3) */

/* ensure we have space for `needed` terms */
static void ensure_term_cap(size_t needed)
{
    if (term_cap >= needed) return;
    size_t new_cap = term_cap ? term_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    /* cast to seq_t* for C++ compatibility */
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms = tmp;
    term_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1 – dummy value */
    /* grow sequence until we have at least `n` terms */
    while (term_count < n) {
        for (;;) {
            uint64_t p = last_checked++;
            if (!isPrime64(p))                     continue;          /* p not prime */
            if (!isPrime64(p + 18))                continue;          /* p+18 not prime */
            unsigned __int128 prod = (unsigned __int128)p * 18 + 1;
            if (prod > UINT64_MAX || !isPrime64((uint64_t)prod)) continue; /* 18p+1 not prime */
            /* p satisfies all conditions */
            ensure_term_cap(term_count + 1);
            terms[term_count++] = (seq_t)p;
            break;
        }
    }
    return terms[n - 1];
}
