/*
 * A235334  Numbers n such that for any positive integers (a, b), if a * b = n
 *          then a + b is a square.
 * https://oeis.org/A235334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 621, u128 n <= 1163,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = 3 for n = 1;
   for n > 1, a(n) is the product (2k²‑1)(2k²+1) where k ≥ 2 and both
   2k²‑1 and 2k²+1 are prime.  This is the characterization of the
   OEIS sequence A235334.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* cache of previously computed terms ------------------------------------ */
static seq_t *cache = NULL;          /* array of terms */
static size_t cache_len = 0;         /* number of stored terms   */
static uint64_t last_k = 0;          /* last k that was examined */

/* append a term to the cache, growing it if necessary ------------------- */
static void cache_append(seq_t val)
{
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
    } else {
        /* grow: double the capacity each time */
        cache = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));
        if (!cache) abort();
    }
    cache[cache_len++] = val;
}

/* fill the cache until it contains at least n terms ------------------- */
static void ensure_up_to(idx_t n)
{
    while (cache_len < n) {
        /* first term is 3 */
        if (cache_len == 0) {
            cache_append((seq_t)3);
            last_k = 1;               /* next k to test is 2 */
            continue;
        }

        /* test next k */
        ++last_k;
        seq_t k = (seq_t)last_k;

        /* compute p = 2k²‑1, q = 2k²+1 */
        seq_t kk = k * k;          /* k²   */
        seq_t p  = kk * 2 - 1;     /* 2k²‑1 */
        seq_t q  = p + 2;          /* 2k²+1 */

        if (is_prime_big(p) && is_prime_big(q)) {
            /* term is n = p * q  = (2k²‑1)(2k²+1) = 4k⁴‑1 */
            seq_t n_val = p * q;
            cache_append(n_val);
        }
    }
}

/* public routine -------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1 */
    ensure_up_to(n);
    return cache[n - 1];
}
