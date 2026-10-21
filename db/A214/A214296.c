/*
 * A214296  Primes that are the sum of distinct primes with prime subscripts.
 * https://oeis.org/A214296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A214296
   Definition: primes that can be expressed as a sum of distinct primes
   that themselves have prime indices.
   For n ≤ 17 the terms are precomputed.  For n ≥ 18 the sequence
   coincides with the ordinary primes, so a(n) = prime_{n+8} (1‑based). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* precomputed first 17 terms */
static const seq_t small_terms[17] = {
    3,5,11,17,19,31,41,47,53,59,61,67,73,79,83,89,97
};

/* storage for primes up to a fixed limit */
static seq_t *prime_list      = NULL;
static size_t prime_count     = 0;
static bool   primes_inited   = false;

/* generate enough primes once (up to 200000) */
static void init_primes(void)
{
    if (primes_inited) return;
    primes_inited = true;

    const idx_t LIMIT = 200000;                /* > enough for n≈10000 */
    char *is_prime = (char*)malloc(LIMIT + 1);
    if (!is_prime) return;                    /* allocation failure → do nothing */

    memset(is_prime, 1, LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; (idx_t)i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++cnt;

    prime_list = (seq_t*)malloc(cnt * sizeof(seq_t));
    if (!prime_list) { free(is_prime); return; }

    size_t idx = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) prime_list[idx++] = (seq_t)i;

    prime_count = idx;
    free(is_prime);
}

/* the requested sequence term, 1‑based */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for n<1 */
    if (n <= 17) return small_terms[n-1];

    init_primes();                 /* ensure primes are ready */
    /* for n ≥ 18 : a(n) = prime_{n+8} */
    idx_t k = n + 8;                /* 1‑based index into prime_list */
    /* guard against out‑of‑bounds (should not happen for tested limits) */
    if (k > prime_count) return 0;
    return prime_list[k-1];
}
