/*
 * A176278  Numbers of the form 6k-1 with the least prime divisor of the form
 *          6m+1.
 * https://oeis.org/A176278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A176278 – numbers 6k − 1 with least prime divisor ≡ 1 (mod 6);
 *            found by sequentially testing candidates 6k−1 using a
 *            pre‑computed smallest‑prime‑factor array
 * Range:     u8 n <= 5, u16 n <= 3129, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------------
 *  Returns the nth term of sequence A176278 (offset 1).
 *  The function is memoised: earlier results are reused on subsequent
 *  calls, independently of the order of requests.
 * -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* ----------------------------------------------------------------
     *  Static state:  SPF array, cache of found terms, and current
     *  position in the candidate list.
     * ---------------------------------------------------------------- */
    static bool initialized = false;
    static uint32_t *spf = NULL;
    static const size_t SPF_LIMIT = 5000000;   /* sufficient for n ≤ 10 000 */
    static seq_t  *cache   = NULL;
    static idx_t   cache_size = 0;
    static idx_t   cache_cap  = 0;
    static idx_t   next_k     = 1;   /* next k to test in 6k-1 */

    /* ----------------------------------------------------------------
     *  One‑time initialisation: build the SPF array and the cache
     * ---------------------------------------------------------------- */
    if (!initialized) {
        /* Build SPF array up to SPF_LIMIT */
        spf = (uint32_t *)calloc(SPF_LIMIT + 1, sizeof(uint32_t));
        for (size_t i = 2; i <= SPF_LIMIT; ++i) {
            if (spf[i] == 0) {
                spf[i] = (uint32_t)i;
                if ((uint64_t)i * i <= SPF_LIMIT) {
                    for (size_t j = i * i; j <= SPF_LIMIT; j += i) {
                        if (spf[j] == 0)
                            spf[j] = (uint32_t)i;
                    }
                }
            }
        }

        /* Initialise cache for found terms */
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_size = 0;
        initialized = true;
    }

    /* ----------------------------------------------------------------
     *  Generate terms until we have at least n of them
     * ---------------------------------------------------------------- */
    while (cache_size < n) {
        uint64_t cand = 6ULL * (uint64_t)next_k - 1ULL;   /* 6k-1 */

        if (cand > SPF_LIMIT) {
            /* For the intended use (n ≤ 10 000) this should never happen */
            break;
        }

        uint32_t spf_c = spf[(size_t)cand];
        /* composite check: SPF != itself (prime numbers have SPF = n) */
        if (spf_c != cand) {
            if ((spf_c % 6U) == 1U) {    /* least prime divisor ≡ 1 (mod 6) */
                /* grow cache if needed */
                if (cache_size == cache_cap) {
                    cache_cap <<= 1;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_size++] = (seq_t)cand;
            }
        }
        ++next_k;
    }

    return cache[n-1];   /* a(1) is at index 0 */
}
