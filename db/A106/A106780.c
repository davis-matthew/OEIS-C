/*
 * A106780  Primes with digit sum = 50.
 * https://oeis.org/A106780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 3300, u64 n <= 3300, u128 n <= 3300,
 *            big any n
 * Helpers:   digit_sum_uint64, isPrime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate primes whose decimal digit sum equals 50 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

#include "../utils/isPrime64.h"          /* bool isPrime64(uint64_t) */
#include "../utils/digit_sum_uint64.h"   /* uint32_t digit_sum_uint64(uint64_t) */

seq_t a(idx_t n)
{
    /* --- static state --- */
    static uint64_t cand = 3;              /* next odd candidate to test */
#ifdef __cplusplus
    static std::vector<seq_t> primes;     /* vector of primes found so far */
#else
    static seq_t *primes = NULL;          /* pointer to dynamically allocated array */
    static size_t cap = 0;                /* current capacity of array */
    static size_t sz = 0;                 /* number of stored primes */
#endif

    /* initialise the C array on first use */
#ifndef __cplusplus
    if (!primes) {
        cap = 64;
        primes = (seq_t *)malloc(cap * sizeof(seq_t));
    }
#endif

    /* generate primes until we have at least n terms */
#ifdef __cplusplus
    while (primes.size() < n) {
        if (isPrime64(cand)) {
            if (digit_sum_uint64(cand) == 50u) {
                primes.push_back(static_cast<seq_t>(cand));
            }
        }
        cand += 2;
    }
#else
    while (sz < n) {
        if (isPrime64(cand)) {
            if (digit_sum_uint64(cand) == 50u) {
                if (sz == cap) {
                    cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(primes, cap * sizeof(seq_t));
                    if (!tmp) { free(primes); abort(); }
                    primes = tmp;
                }
                primes[sz++] = (seq_t)cand;
            }
        }
        cand += 2;
    }
#endif

    /* return the requested term */
#ifdef __cplusplus
    return primes[static_cast<size_t>(n - 1)];
#else
    return primes[n - 1];
#endif
}
