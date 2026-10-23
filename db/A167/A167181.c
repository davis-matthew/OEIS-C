/*
 * A167181  Squarefree numbers such that all prime factors are == 3 mod 4.
 * https://oeis.org/A167181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th squarefree integer all of whose prime factors
 *            are ≡ 3 (mod 4); a(1) = 1. Internally the sequence is generated
 *            eagerly and cached so that any order of calls works
 * Range:     u8 n <= 49, u16 n <= 8270, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>    /* for size_t */

/* static cache for the sequence: 1‑based index, cache_len == number
   of stored terms.  It grows as needed. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* how many terms are stored */
static size_t cache_cap = 0;   /* memory capacity in terms */

/* last integer examined when scanning for terms. */
static uint64_t last_checked = 0ULL;

/* Resize the cache array to at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    if (new_cap < needed) new_cap = needed;

    /* realloc returns void*, so cast explicitly to support C++. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;          /* allocation failed – keep old cache */

    cache  = tmp;
    cache_cap = new_cap;
}

/* Return true if 'num' is squarefree and every prime factor is
   congruent to 3 modulo 4. */
static bool is_special(uint64_t num)
{
    if (num == 1ULL) return true;    /* 1 is accepted by convention */

    /* Even numbers have the factor 2, which is ≡ 2 (mod 4). */
    if ((num & 1ULL) == 0ULL) return false;

    uint64_t n = num;

    /* Trial division by odd primes up to sqrt(n). */
    for (uint64_t p = 3ULL; p <= n / p; p += 2ULL) {
        uint8_t cnt = 0;
        while ((n % p) == 0ULL) {
            n /= p;
            ++cnt;
            if (cnt > 1U) return false;       /* repeated factor → not squarefree */
        }
        if (cnt == 1U && (p & 3ULL) != 3ULL)
            return false;                       /* prime factor not ≡ 3 (mod 4) */
    }

    /* Any remaining factor > 1 is a single prime factor. */
    if (n > 1ULL && (n & 3ULL) != 3ULL)
        return false;

    return true;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below the offset */

    size_t idx = (size_t)n;        /* enforce 64‑bit arithmetic */

    /* Return cached value if already computed. */
    if (idx <= cache_len) return cache[idx - 1];

    /* Generate terms until we reach the required index. */
    while (cache_len < idx) {
        ++last_checked;                /* next candidate integer */

        if (is_special(last_checked)) {
            ensure_cache_capacity(cache_len + 1);
            if (cache_len + 1 > cache_cap) break;   /* allocation failure */

            cache[cache_len] = (seq_t)last_checked;
            ++cache_len;
        }
    }

    /* Return the term if found, otherwise 0 (should not happen). */
    if (idx <= cache_len) return cache[idx - 1];
    return 0;
}
