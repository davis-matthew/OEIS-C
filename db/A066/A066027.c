/*
 * A066027  Numbers k such that the sum of digits of k minus the product of
 *          digits of k is prime.
 * https://oeis.org/A066027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = k such that digit_sum(k) - digit_product(k) is prime
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product, digit_sum, grow_seq_array, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"
#include "../utils/is_prime_small.h"
#include "../utils/grow_seq_array.h"

/* static state for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1;

/* Return the n-th term (n ≥ 1) of OEIS sequence A066027 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n < 1 */

    /* initialise cache on first call */
    if (cache_cap == 0) {
        grow_seq_array(&cache, &cache_cap, 64);  /* reasonable start size */
    }

    while (cache_len < n) {
        /* candidate must fit in idx_t; wrap‐around is ignored for 64‑bit builds */
        idx_t cand = next_candidate++;
        seq_t sk = digit_sum((seq_t)cand);
        seq_t pk = digit_product((seq_t)cand);

        if (pk > sk) continue;          /* negative difference, cannot be prime */

        seq_t diff = sk - pk;
        if (diff < 2) continue;          /* 0 or 1 are not prime */

        if (!is_prime_small(diff)) continue;

        /* grow cache if needed */
        if (cache_len + 1 > cache_cap) {
            grow_seq_array(&cache, &cache_cap, cache_cap * 2);
        }
        cache[cache_len++] = (seq_t)cand;
    }

    return cache[(size_t)(n - 1)];        /* a(1) is first cached element */
}
