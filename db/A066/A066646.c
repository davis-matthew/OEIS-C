/*
 * A066646  Arrange the permutations of {1..m} in lexicographic order.
 *          Sequence gives indices of permutations of orders 1 or 2.
 * https://oeis.org/A066646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based implementation: iterate integers k, compute the k‑th
   lexicographic permutation of {1..m} (where m! ≥ k) and test whether
   that permutation is an involution (order 1 or 2).  The first such
   indices form the sequence.  Results are memoised so that repeated
   calls return quickly. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* The types seq_t and idx_t are provided by the build environment. */
seq_t a(idx_t n)
{
    /* ---------- static data shared between calls ---------- */
    static seq_t *cache = NULL;          /* array of a(i) values       */
    static size_t cache_len = 0;         /* number of values stored   */
    static size_t cache_cap = 0;         /* current capacity of cache */
    static unsigned long long last_k_processed = 0; /* last k examined */

    /* precompute factorials up to 20! (fits in 64‑bit) */
    static unsigned long long fact[21];
    static bool fact_inited = false;
    if (!fact_inited) {
        fact[0] = 1;
        for (int i = 1; i < 21; ++i)
            fact[i] = fact[i - 1] * (unsigned long long)i;
        fact_inited = true;
    }

    /* initialise cache on first use */
    if (cache_len == 0) {
        cache_cap = 1024;                       /* initial size */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate terms until we have computed a(n) for the requested n */
    while (cache_len < n) {
        unsigned long long k = last_k_processed + 1; /* next index to test */

        /* find minimal m such that m! ≥ k */
        int m = 1;
        while (fact[m] < k) ++m;

        /* convert k (1‑based) to the lexicographic permutation of {1..m} */
        unsigned long long r = k - 1;          /* zero‑based rank   */
        int avail[20];                         /* elements still free */
        for (int i = 0; i < m; ++i) avail[i] = i + 1;
        int perm[20];                          /* resulting permutation, 0‑based indices */
        for (int i = 0; i < m; ++i) {
            unsigned long long f = fact[m - 1 - i];
            unsigned long long idx = r / f;
            r %= f;
            perm[i] = avail[idx];
            /* remove chosen element from avail */
            for (int j = idx; j < m - 1 - i; ++j)
                avail[j] = avail[j + 1];
        }

        /* test whether perm is an involution: perm[perm[i]-1] == i+1 for all i */
        bool is_involution = true;
        for (int i = 0; i < m; ++i) {
            int image = perm[i];
            int preimage = perm[image - 1];
            if (preimage != (i + 1)) {
                is_involution = false;
                break;
            }
        }

        if (is_involution) {
            /* store the index k into the cache */
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)k;
        }

        last_k_processed = k; /* advance to next integer for the next loop */
    }

    return cache[n - 1]; /* cache is 0‑based, n is 1‑based */
}
