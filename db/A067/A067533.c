/*
 * A067533  Numbers k such that both k - tau(k) and k + tau(k) are prime where
 *          tau(k) = A000005(k).
 * https://oeis.org/A067533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 638, u32 n <= 795, u64 n <= 1128,
 *            u128 n <= 1128, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A067533:  numbers k such that k‑τ(k) and k+τ(k)
 *  are both prime.  Indexed with a(1)=5.  The routine
 *  builds a sieve of smallest prime factors up to a
 *  generous limit and then searches for terms on demand.
 *  It memoises found terms so that calls are O(1) in
 *  the number of terms already observed.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */

#if !defined(SIEVE_LIMIT)
/* at least enough to generate the first 10 000 terms */
#define SIEVE_LIMIT     5000000u
#endif

/* smallest prime factor array, sieve size in spf[0..SIEVE_LIMIT]           */
static uint32_t *spf          = NULL;
/* cached sequence terms                                             */
static seq_t   *terms         = NULL;
/* current capacity and count of stored terms                        */
static size_t  terms_cap      = 0;
static size_t  terms_cnt      = 0;
/* initialisation flag                                               */
static bool    initialized    = false;

/* ------------------------------------------------------------------ */
/* Linear sieve to fill spf[] up to SIEVE_LIMIT                      */
/* ------------------------------------------------------------------ */
static void build_spf(void)
{
    size_t i, p, primes_cnt;
    size_t *primes = (size_t *)malloc((SIEVE_LIMIT / 10u) * sizeof(size_t));
    spf = (uint32_t *)calloc(SIEVE_LIMIT + 1, sizeof(uint32_t));
    primes_cnt = 0;

    for (i = 2; i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            primes[primes_cnt++] = i;
        }
        for (p = 0; p < primes_cnt; ++p) {
            size_t k = i * primes[p];
            if (k > SIEVE_LIMIT) break;
            spf[k] = (uint32_t)primes[p];
            if (primes[p] == spf[i]) break;
        }
    }
    free(primes);
}

/* ------------------------------------------------------------------ */
/* Return the nth term (1‑based).  Calls may be in any order.  The
 * routine runs linear in the number of terms required.            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) {
        initialized = true;
        build_spf();

        terms_cap = 64;
        terms     = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }

    /* Generate terms up to index n if needed                     */
    for (idx_t cand = (terms_cnt > 0 ? (idx_t)terms[terms_cnt - 1] + 1 : 2);
         terms_cnt < n;
         ++cand)
    {
        /* divisor count τ(cand) via the spf array                  */
        seq_t tau = 1;
        idx_t tmp = cand;
        while (tmp > 1) {
            uint32_t p = spf[(size_t)tmp];
            uint8_t   cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            tau *= (seq_t)(cnt + 1);
        }

        seq_t a_minus = (seq_t)cand - tau;
        seq_t a_plus  = (seq_t)cand + tau;

        if (is_prime_big(a_minus) && is_prime_big(a_plus)) {
            if (terms_cnt >= terms_cap) {
                terms_cap *= 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_cnt++] = (seq_t)cand;
        }
    }

    return terms[n - 1];
}
