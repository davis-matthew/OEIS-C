/*
 * A065867  Primes which are the sum of a prime number of consecutive primes.
 * https://oeis.org/A065867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime that can be written as a sum of a prime
 *            number of consecutive primes. The function computes this by
 *            building a sieve of primes up to an increasing limit until at
 *            least n terms are found. Results are cached so that repeated
 *            calls are fast
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 70 terms through n = 895 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include "../utils/mem.h"

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

/* cached sequence */
static seq_t *g_cache = NULL;
static size_t g_cache_size = 0;   /* number of cached terms                     */
static idx_t g_limit = 0;         /* current sieve upper bound (in primes)       */

/* build a sieve of primes up to ‘limit’ and return:
        *primes_out : array of primes (uint32_t)
        *cnt_out    : number of primes
        *flag_out   : boolean array where flag[n]==1 iff n is prime
   All arrays are malloc’d and must be freed by the caller. */
static void
build_sieve(idx_t limit, uint32_t **primes_out, size_t *cnt_out, uint8_t **flag_out)
{
    uint8_t *flag = (uint8_t *)malloc((size_t)(limit + 1));
    if (!flag) { *primes_out = NULL; *cnt_out = 0; *flag_out = NULL; return; }

    memset(flag, 1, (size_t)(limit + 1));
    flag[0] = flag[1] = 0;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (flag[(size_t)i]) {
            for (idx_t j = i * i; j <= limit; j += i) flag[(size_t)j] = 0;
        }
    }

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (flag[(size_t)i]) ++cnt;

    uint32_t *primes = (cnt > 0) ? (uint32_t *)malloc(cnt * sizeof(uint32_t)) : NULL;
    if (primes) {
        size_t k = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (flag[(size_t)i]) primes[k++] = (uint32_t)i;
    }

    *primes_out = primes;
    *cnt_out    = cnt;
    *flag_out   = flag;
}

/* recompute the cache so that it contains at least ‘needed’ terms.
   The function doubles the sieve limit until enough terms have been found. */
static void
recompute(size_t needed)
{
    if (needed <= g_cache_size) return;

    idx_t limit = (g_limit == 0) ? 2000000 : g_limit;

    while (1) {
        uint32_t *primes  = NULL;
        size_t   primes_cnt = 0;
        uint8_t *is_prime = NULL;   /* flag array for sieve */

        build_sieve(limit, &primes, &primes_cnt, &is_prime);
        if (!primes || !is_prime) break;          /* allocation failure */

        /* prefix sums of primes (as seq_t) */
        seq_t *cum = mem_new(seq_t, (primes_cnt + 1));
        if (!cum) { free(primes); free(is_prime); break; }
        cum[0] = (seq_t)0;
        for (size_t i = 1; i <= primes_cnt; ++i)
            cum[i] = cum[i-1] + (seq_t)primes[i-1];

        /* collect candidate sums */
        size_t tmp_cap = 1024;
        seq_t *tmp = mem_new(seq_t, tmp_cap);
        size_t tmp_cnt = 0;
        if (!tmp) { free(primes); free(is_prime); mem_free(cum); break; }

        for (size_t start = 0; start < primes_cnt; ++start) {
            for (size_t len = 2; start + len <= primes_cnt; ++len) {
                /* discard if length not prime */
                if (!is_prime[len]) continue;
                seq_t sum = cum[start + len] - cum[start];
                if (sum > (seq_t)limit) break;           /* sums grow with len */
                /* sum must be a prime (and <= limit) */
                if (is_prime[(size_t)sum]) {
                    if (tmp_cnt == tmp_cap) {
                        tmp_cap <<= 1;
                        seq_t *tmp2 = mem_resize(seq_t, tmp, tmp_cap);
                        if (!tmp2) { mem_free(tmp); mem_free(cum); free(primes); free(is_prime); break; }
                        tmp = tmp2;
                    }
                    tmp[tmp_cnt++] = sum;
                }
            }
            if (tmp_cnt < tmp_cap) { /* inner loop terminated by failure; continue outer loop */ }
        }

        mem_free(cum);
        free(primes);
        free(is_prime);

        /* sort and deduplicate */
        if (tmp_cnt > 0) {
            mem_qsort(tmp, tmp_cnt, sizeof(seq_t), cmp_seq_t);
            size_t uniq_cnt = 1;
            for (size_t i = 1; i < tmp_cnt; ++i)
                if (tmp[i] != tmp[uniq_cnt-1]) tmp[uniq_cnt++] = tmp[i];

            /* replace cache if larger */
            if (uniq_cnt > g_cache_size) {
                seq_t *new_cache = mem_new(seq_t, uniq_cnt);
                if (new_cache) {
                    mem_copy(new_cache, tmp, uniq_cnt * sizeof(seq_t));
                    mem_free(g_cache);
                    g_cache = new_cache;
                    g_cache_size = uniq_cnt;
                }
            }
            mem_free(tmp);
        } else {
            mem_free(tmp);
        }

        /* check if enough terms found */
        if (g_cache_size >= needed) break;

        /* increase limit and try again */
        limit <<= 1;
    }
    g_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, return 0 */
    recompute((size_t)n);
    return g_cache[n-1];
}
