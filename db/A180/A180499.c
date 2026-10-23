/*
 * A180499  n^3 + n-th cubefree number.
 * https://oeis.org/A180499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 + A004709(n) where A004709(n) is the n‑th * cubefree
 *            number. (OEIS A180499)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Global state ----------------------------------------------------------- */
static seq_t *cubefree_cache = NULL;   /* array of cubefree numbers       */
static size_t cubefree_cnt   = 0;       /* how many are stored              */
static size_t cubefree_cap   = 0;       /* allocated capacity               */

static uint32_t *prime_list  = NULL;   /* primes for factorisation         */
static size_t    prime_cnt   = 0;       /* how many primes are stored       */
static size_t    prime_cap   = 0;       /* allocated capacity of primes     */
static uint64_t  primes_last = 1;      /* largest integer for which
                                         * primes have been generated      */

static uint64_t  last_checked = 0;     /* last integer checked for cubefree */

/* Utility ----------------------------------------------------------------- */

/* Grow the prime list so that every prime ≤ limit is present. */
static void grow_primes_upto(uint64_t limit)
{
    if (limit <= primes_last)
        return;

    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        uint32_t *tmp  = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
        prime_list = tmp;
        prime_cap  = new_cap;
    }

    for (uint64_t n = primes_last + 1; n <= limit; ++n) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * p > n)
                break;
            if (n % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                uint32_t *tmp  = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
                prime_list = tmp;
                prime_cap  = new_cap;
            }
            prime_list[prime_cnt++] = (uint32_t)n;
        }
    }
    primes_last = limit;
}

/* Ensure that the cache contains at least 'need' cubefree numbers. */
static void grow_cubefree_cache(idx_t need)
{
    while (cubefree_cnt < (size_t)need) {
        ++last_checked;

        /* We need primes up to the cube root of last_checked.
         * Requesting all primes up to last_checked is safe enough
         * for the required ranges. */
        grow_primes_upto(last_checked);

        uint64_t x = last_checked;
        bool cubefree = true;
        for (size_t i = 0; i < prime_cnt;
             ++i) {
            uint32_t p = prime_list[i];
            uint64_t p3 = (uint64_t)p * p * p;
            if (p3 > x)
                break;
            int exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
                if (exp >= 3) {
                    cubefree = false;
                    break;
                }
            }
            if (!cubefree)
                break;
        }

        if (cubefree) {
            if (cubefree_cnt == cubefree_cap) {
                size_t new_cap = cubefree_cap ? cubefree_cap * 2 : 16;
                seq_t *tmp     = (seq_t *)realloc(cubefree_cache,
                                                new_cap * sizeof(seq_t));
                cubefree_cache = tmp;
                cubefree_cap   = new_cap;
            }
            cubefree_cache[cubefree_cnt++] = (seq_t)last_checked;
        }
    }
}

/* Public function -------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                           /* undefined below offset */

    /* Make sure that the n‑th cubefree number is cached. */
    grow_cubefree_cache((size_t)n);

    seq_t n_seq = (seq_t)n;
    seq_t cube  = n_seq * n_seq * n_seq;
    seq_t cf    = cubefree_cache[(size_t)n - 1];   /* 0‑based indexing */

    return cube + cf;
}
