/*
 * A172166  Partial sums of A028388 good primes (version 2).
 * https://oeis.org/A172166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A172166: partial sums of A028388 good primes (version 2)
   a(n) = sum of the first n good primes.
   A good prime p_k (k≥3) satisfies p_k^2 > p_{k-i}·p_{k+i} for all
   1 ≤ i ≤ k-1, where p_j denotes the j‑th prime in the ordinary
   prime sequence. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------- */
/* Global data                                                    */
/* ------------------------------------------------------------- */

/* Prime list (1‑based in comments, but 0‑indexed in array)   */
static uint32_t *g_primes   = NULL;   /* all primes, 0‑based          */
static size_t   g_primes_len = 0;      /* number of primes stored      */
static size_t   g_primes_cap = 0;      /* array capacity                */

/* Good primes & cumulative sums                                 */
static uint32_t *g_good    = NULL;   /* good primes                 */
static seq_t   *g_cumsum   = NULL;   /* cumulative sums of them     */
static size_t   g_good_len = 0;      /* number of good primes       */
static size_t   g_good_cap = 0;      /* array capacity               */

/* Highest prime index that has been examined for goodness      */
static size_t   g_tested_up_to = 0;

/* ------------------------------------------------------------- */
/* Helper functions                                               */
/* ------------------------------------------------------------- */

/* Ensure that at least 'need' primes are available.              */
/* The sieve is built incrementally by simple trial division –   */
/* fast enough for the limits required (≈ 300 000 for n ≤ 10 000). */
static void ensure_primes_upto_index(size_t need)
{
    if (need <= g_primes_len) return;

    if (g_primes_cap < need) {
        size_t new_cap = g_primes_cap ? g_primes_cap * 2 : 1024;
        if (new_cap < need) new_cap = need;
        uint32_t *tmp = (uint32_t*)realloc(g_primes, new_cap * sizeof(uint32_t));
        if (!tmp) exit(EXIT_FAILURE);
        g_primes = tmp;
        g_primes_cap = new_cap;
    }

    if (g_primes_len == 0) {          /* seed with first primes */
        g_primes[g_primes_len++] = 2;
        g_primes[g_primes_len++] = 3;
    }

    uint32_t cand = g_primes[g_primes_len - 1] + 2;
    while (g_primes_len < need) {
        bool prime = true;
        uint32_t r = (uint32_t)((uint64_t)cand * cand);  /* overflow safe for 32bit */
        for (size_t i = 0; i < g_primes_len; ++i) {
            uint32_t p = g_primes[i];
            if ((uint64_t)p * p > cand) break;
            if (cand % p == 0) { prime = false; break; }
        }
        if (prime) {
            g_primes[g_primes_len++] = cand;
        }
        cand += 2;
    }
}

/* Ensure that at least 'need' good primes are known.            */
static void ensure_good_primes(size_t need)
{
    if (need <= g_good_len) return;

    if (g_good_cap < need) {
        size_t new_cap = g_good_cap ? g_good_cap * 2 : 128;
        if (new_cap < need) new_cap = need;
        uint32_t *tmpg = (uint32_t*)realloc(g_good, new_cap * sizeof(uint32_t));
        if (!tmpg) exit(EXIT_FAILURE);
        g_good = tmpg;
        seq_t *tmpc = (seq_t*)realloc(g_cumsum, new_cap * sizeof(seq_t));
        if (!tmpc) exit(EXIT_FAILURE);
        g_cumsum = tmpc;
        g_good_cap = new_cap;
    }

    for (size_t j = g_tested_up_to + 1; need > g_good_len; ++j) {
        size_t required_prime_index = 2 * j - 1;          /* need primes up to this index */
        ensure_primes_upto_index(required_prime_index);

        if (j < 3) {        /* 2 and 3 can never be good */
            g_tested_up_to = j;
            continue;
        }

        uint32_t pk = g_primes[j - 1];
        seq_t pk_sq = (seq_t)pk * (seq_t)pk;
        bool good = true;

        for (size_t i = 1; i < j; ++i) {
            uint32_t left  = g_primes[j - i - 1];
            uint32_t right = g_primes[j + i - 1];
            seq_t prod = (seq_t)left * (seq_t)right;
            if (pk_sq <= prod) { good = false; break; }
        }

        if (good) {
            g_good[g_good_len] = pk;
            if (g_good_len == 0)
                g_cumsum[g_good_len] = (seq_t)pk;
            else
                g_cumsum[g_good_len] = g_cumsum[g_good_len - 1] + (seq_t)pk;
            ++g_good_len;
        }

        g_tested_up_to = j;
    }
}

/* ------------------------------------------------------------- */
/* Public function                                               */
/* ------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n ≤ 0 */

    /* Compute the first n good primes and their sum */
    ensure_good_primes((size_t)n);

    /* Return the partial sum */
    return g_cumsum[(size_t)n - 1];
}
