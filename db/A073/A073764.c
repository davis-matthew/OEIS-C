/*
 * A073764  a(n) = least number x such that A045763(x)=n or 0 if no such
 *          number exists.
 * https://oeis.org/A073764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   compute_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = least x with A045763(x)=n by sieving up to LIMIT. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/compute_spf.h"

#ifndef SEQ_T_DEFINED
#define SEQ_T_DEFINED
/* seq_t and idx_t are defined by the build system. */
/* For example: typedef uint64_t seq_t; typedef uint64_t idx_t; */
#endif

/* Maximum x to sieve.  This value exceeds the x needed for n up to about 10 000
   and gives a good safety margin. */
#define LIMIT   2000000U

/* Static tables used after the first call. */
static bool      initialized = false;
static uint32_t *spf_arr     = NULL;  /* smallest prime factor */
static seq_t   *ans_arr     = NULL;  /* minimal x for each A045763 value */

/* Initialise the cache once.  After this, a(idx_t n) can be called. */
static void init_cache(void)
{
    if (initialized) return;

    size_t lim = LIMIT;
    spf_arr = (uint32_t*)calloc(lim + 1, sizeof(uint32_t));
    ans_arr = (seq_t*)calloc(lim + 1, sizeof(seq_t));

    compute_spf(lim, spf_arr);

    /* For each x compute A045763(x) = x - φ(x) - τ(x) + 1 and store the
       minimal x for each resulting value. */
    for (size_t x = 2; x <= lim; ++x) {
        /* Compute φ(x) and τ(x) by prime factorisation using spf. */
        seq_t phi = (seq_t)x;
        uint32_t tau = 1;
        size_t n = x;
        while (n > 1) {
            uint32_t p = spf_arr[n];
            uint32_t exp = 0;
            while (n % p == 0) { n /= p; ++exp; }
            phi = phi / p * (p - 1);
            tau *= (exp + 1);
        }

        seq_t val = (seq_t)x - phi - (seq_t)tau + 1;
        if (val > 0 && val <= (seq_t)lim) {
            size_t idx = (size_t)val;
            if (ans_arr[idx] == 0) ans_arr[idx] = (seq_t)x;
        }
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    init_cache();

    if (n == 0 || n > (idx_t)LIMIT) return (seq_t)0;
    return ans_arr[(size_t)n];
}
