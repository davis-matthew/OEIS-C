/*
 * A229272  Numbers n for which n' + n and n' - n are both prime, n' being the
 *          arithmetic derivative of n.
 * https://oeis.org/A229272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 80, u32 n <= 300, u64 n <= 300, u128 n <= 300,
 *            big any n
 * Helpers:   factor_u64, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A229272: Numbers n for which n' + n and n' - n are both prime
 * (n' = arithmetic derivative of n).
 * The function searches numbers sequentially, caching found terms.
 * It uses factor_u64 for prime factorization and isPrimeGeneric for
 * primality testing.  Candidate numbers are tested in increasing order.
 * For arbitrary-precision seq_t the implementation is exact; for
 * uint64_t it remains correct as the intermediate values are bounded
 * by 64 bits for the range of n considered.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/isPrimeGeneric.h"

static seq_t *seq_cache = NULL;   /* cached terms of the sequence   */
static size_t seq_cap   = 0;      /* allocated capacity              */
static size_t seq_len   = 0;      /* current number of cached terms  */
static uint64_t next_check = 2;   /* next integer to test           */

/* Test whether a given uint64_t number satisfies the definition
 * of the sequence.  It computes the arithmetic derivative n' and
 * checks that n' + n and n' - n are both prime.
 */
static bool is_candidate(uint64_t n)
{
    if (n < 2)
        return false;

    /* Factor n.  factor_u64 returns the prime factors and their exponents. */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    /* Compute the arithmetic derivative n' = n * Σ(e_i / p_i) = Σ(n / p_i * e_i). */
    seq_t nprime = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t part = (seq_t)n / primes[i];
        nprime += part * exps[i];
    }

    if (nprime <= (seq_t)n)          /* must have n' > n for n' - n > 0 */
        return false;

    seq_t sp = nprime + (seq_t)n;     /* n' + n  */
    seq_t sm = nprime - (seq_t)n;     /* n' - n  */

    if (sp <= 1 || sm <= 1)
        return false;

    return isPrimeGeneric(sp) && isPrimeGeneric(sm);
}

/* Return the n-th term of A229272 (1‐based index). */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined, but avoid underflow */
        return 0;

    /* Ensure the cache holds at least n terms. */
    while (seq_len < n) {
        /* Search the next candidate. */
        uint64_t cand = next_check++;
        if (!is_candidate(cand))
            continue;

        /* Expand cache if needed. */
        if (seq_len >= seq_cap) {
            size_t new_cap = seq_cap ? seq_cap << 1 : 16;
            seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            if (!tmp)          /* allocation failure – terminate */
                abort();
            seq_cache = tmp;
            seq_cap = new_cap;
        }

        seq_cache[seq_len++] = (seq_t)cand;
    }

    return seq_cache[n - 1];
}
