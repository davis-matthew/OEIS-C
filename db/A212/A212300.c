/*
 * A212300  Median prime factor of 1..n.
 * https://oeis.org/A212300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 1052, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A212300:  a(n) = smallest prime p such that at least ceil(n/2)
   numbers in 1..n have all prime factors ≤ p (largest prime factor ≤ p). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static seq_t *cache = NULL;   /* cache[1..current_max] */
static idx_t cache_size = 0;  /* number of allocated entries (current_max+1) */

/* Build a new cache up to n (inclusive).  All previous data is discarded. */
static void extend_to(idx_t n)
{
    if (n < 1 || n < cache_size)
        return;  /* nothing to do */

    /* 1. Compute largest prime factor for 1..n using a simple sieve. */
    uint64_t *lpf = (uint64_t *)calloc((size_t)(n + 1), sizeof(uint64_t));
    if (!lpf) return;
    lpf[1] = 1;

    for (idx_t i = 2; i <= n; ++i) {
        if (lpf[i] == 0) {          /* i is prime */
            for (idx_t j = i; j <= n; j += i)
                lpf[j] = i;         /* overwrite with larger prime when found */
        }
    }

    /* 2. Build list of primes up to n. */
    uint64_t *primes = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    if (!primes) { free(lpf); return; }
    size_t primes_cnt = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (lpf[i] == i) primes[primes_cnt++] = (uint64_t)i;

    /* 3. Allocate new cache and frequency array. */
    seq_t *new_cache = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!new_cache) { free(lpf); free(primes); return; }

    /* freq[lp] holds how many numbers ≤ current i have largest prime factor = lp. */
    size_t *freq = (size_t *)calloc((size_t)(n + 1), sizeof(size_t));
    if (!freq) { free(lpf); free(primes); free(new_cache); return; }

    /* 4. Compute cache values incrementally. */
    for (idx_t i = 1; i <= n; ++i) {
        uint64_t lp = lpf[i];
        ++freq[lp];

        idx_t needed = (i + 1) / 2;      /* ceil(i/2) */
        seq_t ans = 2;                   /* default prime is 2 */
        seq_t cum = (seq_t)freq[1];      /* include numbers with lpf = 1 (only 1) */

        if (cum < needed) {
            for (size_t k = 0; k < primes_cnt; ++k) {
                cum += (seq_t)freq[primes[k]];
                if (cum >= needed) {
                    ans = (seq_t)primes[k];
                    break;
                }
            }
        }
        new_cache[i] = ans;
    }

    /* 5. Replace old cache and deallocate temporary arrays. */
    free(cache);
    cache = new_cache;
    cache_size = n + 1;  /* we ignore cache[0] */

    free(lpf);
    free(primes);
    free(freq);
}

/* Public implementation of the sequence. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* undefined for n < 1; harmless */
    extend_to(n);
    return cache[n];
}
