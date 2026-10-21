/*
 * A075339  a(1) = 1, a(2n) = smallest composite number obtained by adding a
 *          prime to a(2n-1). a(2n+1) = the smallest prime obtained by adding
 *          a composite number to a(2n).
 * https://oeis.org/A075339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A075339
   (see OEIS entry A075339 for the definition). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *cache   = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */
    if (n <= cache_len) return cache[n-1];

    ensure_cache_cap((size_t)n);

    for (idx_t idx = cache_len + 1; idx <= n; ++idx) {
        if (idx == 1) {            /* a(1) = 1 */
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t prev = cache[(size_t)(idx-2)];   /* a(idx-1) */
        seq_t val = 0;

        if ((idx & 1) == 0) {      /* even: add a prime until composite */
            for (idx_t p = 2; ; ++p) {
                if (!is_prime(p)) continue;   /* skip non‑primes */
                seq_t sum = prev + (seq_t)p;
                if (sum <= (seq_t)1) continue;
                if (!is_prime((idx_t)sum)) {   /* composite found */
                    val = sum;
                    break;
                }
            }
        } else {                   /* odd >1: add a composite until prime */
            for (idx_t c = 4; ; ++c) {
                if (is_prime(c)) continue;   /* skip primes */
                seq_t sum = prev + (seq_t)c;
                if (is_prime((idx_t)sum)) {   /* prime found */
                    val = sum;
                    break;
                }
            }
        }

        cache[(size_t)(idx-1)] = val;
    }

    cache_len = n;
    return cache[n-1];
}
