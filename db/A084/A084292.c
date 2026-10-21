/*
 * A084292  a(n) = 6n + A054904(n).
 * https://oeis.org/A084292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 112, u32 n <= 696, u64 n <= 724,
 *            u128 n <= 724, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS Formula: a(n) = 6*n + A054904(n)
    This implements the search for the smallest composite y > 6*n
    satisfying sigma(y) - sigma(y-6*n) = 6*n.  */
/*  Includes: stdint.h, stdlib.h, stdbool.h, stddef.h */ 
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Forward declarations of helper functions */
static void ensure_sieve(size_t need);
static seq_t sigma_from_spf(uint64_t n);

/* SPF array (least prime divisor) and its current bound */
static uint32_t *spf = NULL;
static size_t spf_len = 0;

/* Ensure that spf contains least prime factor for every number up to
   at least 'need'.  The sieve is a simple linear (Sieve of Eratosthenes)
   implementation; it is reallocated from scratch when extended. */
static void ensure_sieve(size_t need)
{
    if (need <= spf_len)                /* already sufficient */
        return;

    free(spf);                           /* discard the old table */
    spf_len = need + 1;
    spf = (uint32_t *)malloc(spf_len * sizeof(uint32_t));
    if (!spf)
        exit(EXIT_FAILURE);

    for (size_t i = 0; i < spf_len; ++i)
        spf[i] = (uint32_t)i;            /* spf[i] = i itself */

    /* Linear sieve for least prime factor */
    for (size_t i = 2; i * i < spf_len; ++i) {
        if (spf[i] == (uint32_t)i) {     /* i is prime */
            for (size_t j = i * i; j < spf_len; j += i) {
                if (spf[j] == (uint32_t)j)
                    spf[j] = (uint32_t)i;
            }
        }
    }
}

/* Compute the sum-of-divisors function σ(n) using the precomputed SPF table.
   It works for n that fits in a 64‑bit unsigned integer and is (assumed)
   less than spf_len.  The result is returned as seq_t. */
static seq_t sigma_from_spf(uint64_t n)
{
    seq_t sum = 1;
    while (n > 1) {
        uint32_t p = spf[(size_t)n];         /* least prime divisor */
        uint64_t power = 1;
        int e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
            power *= p;                       /* p^e */
        }
        /* term = (p^(e+1) - 1) / (p - 1) */
        __int128 numerator = (__int128)power * p - 1;
        seq_t term = (seq_t)(numerator / (p - 1));
        sum *= term;
    }
    return sum;
}

/* --------------------------------------------------------------------- */
/*  Public function implementing sequence A084292.                */
/*  External linkage: seq_t a(idx_t n)                         */
seq_t a(idx_t n)
{
    /* Initialise the SPF sieve once, with a moderate upper bound. */
    static bool initialized = false;
    if (!initialized) {
        ensure_sieve(2000000);              /* initial range up to 2 000 000 */
        initialized = true;
    }

    /* 6*n as a 64-bit unsigned integer.  It will be used for indexing. */
    uint64_t target = (uint64_t)n * 6ULL;

    /* Search for the minimal composite y = target + k with k >= 1. */
    for (uint64_t k = 1; ; ++k) {
        uint64_t y = target + k;                 /* candidate y */

        /* Grow the sieve if necessary to cover this number. */
        if ((size_t)y >= spf_len)
            ensure_sieve(y * 2 + 1000);

        seq_t sigma_k = sigma_from_spf(k);
        seq_t sigma_y = sigma_from_spf(y);

        /* Check the main condition and that y is composite. */
        if (sigma_y - sigma_k == target && y > 1 && spf[(size_t)y] != (uint32_t)y)
            return (seq_t)y;

        /* The loop is guaranteed to terminate because the OEIS
           definition guarantees a solution for each n. */
    }
}
