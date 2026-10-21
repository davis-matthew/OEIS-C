/*
 * A075340  a(1) = 1; a(2n) is the smallest prime == 1 mod (a(2n-1)) and
 *          a(2n+1) is the smallest composite number == 1 (mod a(2n)).
 * https://oeis.org/A075340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 23, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A075340: a(1)=1; for n>=2,
   a(2k)   = smallest prime p such that p ≡ 1 (mod a(2k-1)),
   a(2k+1) = smallest composite c such that c ≡ 1 (mod a(2k)).
   This implementation caches results so calls can come in any order. */
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* static cache for computed terms */
static seq_t *cache = NULL;
static size_t cap = 0;
static size_t sz  = 0;

/* enlarge cache to hold at least 'need' elements */
static void ensure_capacity(size_t need)
{
    if (need <= cap) return;
    size_t newcap = cap ? cap << 1 : 16;
    while (newcap < need) newcap <<= 1;
    seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!newptr) abort();           /* out of memory */
    cache = newptr;
    cap   = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined below offset */
    /* compute terms up to n */
    if ((size_t)n > sz) {
        ensure_capacity((size_t)n);
        for (idx_t i = (idx_t)(sz + 1); i <= n; ++i) {
            size_t idx = (size_t)(i - 1);   /* 0‑based index */
            if (i == 1) {
                cache[idx] = (seq_t)1;
                continue;
            }
            seq_t prev = cache[idx - 1];
            seq_t cand = prev + (seq_t)1;    /* cand ≡ 1 (mod prev) */

            if ((i & 1ULL) == 0) {          /* even index -> prime */
                while (!is_prime_big(cand)) cand += prev;
            } else {                        /* odd index -> composite */
                while (is_prime_big(cand))  cand += prev;
            }
            cache[idx] = cand;
        }
        sz = (size_t)n;
    }
    return cache[(size_t)(n - 1)];
}
