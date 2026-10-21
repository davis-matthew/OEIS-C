/*
 * A205153  Least k such that n divides s(k)-s(j) for some j<k, where
 *          s(j)=(prime(j+1) + prime(j+2))/2.
 * https://oeis.org/A205153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that n divides s(k)-s(j) for some j<k, where
 *            s(j) = (prime(j+1)+prime(j+2))/2. Process: find the first
 *            repeated residue of s(k) mod n
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1, n>=1 in normal use */

    /* -------- memoization cache for computed a(n) -------- */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    if ((size_t)n > cache_cap) {
        size_t new_cap = (n > 1) ? n * 2 : 2;
        seq_t *tmp = (seq_t*)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();
        for (size_t i = cache_cap + 1; i <= new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }
    if (cache[n] != 0) return cache[n];

    /* -------- prime list generation (0‑based primes[0]=2) -------- */
    static uint32_t *primes = NULL;
    static size_t primes_len = 0, primes_cap = 0;
    if (primes_len == 0) {
        primes_cap = 16;
        primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
        if (!primes) abort();
        primes[0] = 2;
        primes[1] = 3;
        primes_len = 2;
    }

    /* We need primes up to index n+3 (1‑based), i.e., array size at least n+4. */
    size_t need_idx = n + 3;
    while (primes_len <= need_idx) {
        uint32_t candidate = primes[primes_len - 1] + 2;
        bool is_prime;
        do {
            is_prime = true;
            uint32_t sq = (uint32_t)floor(sqrt((double)candidate));
            for (size_t i = 0; i < primes_len && primes[i] <= sq; ++i) {
                if (candidate % primes[i] == 0) { is_prime = false; break; }
            }
            if (!is_prime) candidate += 2;
        } while (!is_prime);

        if (primes_len == primes_cap) {
            primes_cap *= 2;
            uint32_t *tmp = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
            if (!tmp) abort();
            primes = tmp;
        }
        primes[primes_len++] = candidate;
    }

    /* -------- array of seen residues -------- */
    static uint8_t *seen = NULL;
    static size_t seen_cap = 0;
    if ((size_t)n > seen_cap) {
        uint8_t *tmp = (uint8_t*)realloc(seen, (size_t)n * sizeof(uint8_t));
        if (!tmp) abort();
        seen = tmp;
        seen_cap = n;
    }
    memset(seen, 0, (size_t)n * sizeof(uint8_t));

    /* -------- search for first repeated residue -------- */
    seq_t answer = 0;
    for (idx_t k = 1; k <= n + 1; ++k) {
        seq_t p1 = (seq_t)primes[(size_t)k];     /* (k+1)-th prime   */
        seq_t p2 = (seq_t)primes[(size_t)k + 1]; /* (k+2)-th prime   */
        seq_t s = (p1 + p2) >> 1;                 /* (p1+p2)/2       */
        seq_t rem = s % (seq_t)n;
        idx_t r = (idx_t)rem;
        if (seen[r]) { answer = k; break; }
        seen[r] = 1;
    }
    if (answer == 0) answer = n + 1;  /* safety, though it should never happen */

    cache[n] = answer;
    return answer;
}
