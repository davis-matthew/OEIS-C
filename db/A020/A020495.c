/*
 * A020495  Neither square nor square + prime.
 * https://oeis.org/A020495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   append_candidate, is_prime_big, isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that are neither a perfect square nor representable as a
   square plus a prime.  The sequence is constructed by searching
   the positive integers, discarding squares, discarding primes
   (since p = 0^2 + p), and discarding any n for which n - t^2
   is prime for some integer t ≥ 1.  The remaining numbers form
   the sequence A020495. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/isqrt_seq.h"
#include "../utils/append_candidate.h"

/* Static cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
/* Next number to test */
static seq_t next_candidate = 2;

seq_t a(idx_t n)
{
    /* Ensure we have computed at least n terms */
    while (cache_len < n) {
        seq_t cand = next_candidate++;          /* candidate under test */

        /* Skip perfect squares */
        seq_t sqrt_cand = isqrt_seq(cand);
        if (sqrt_cand * sqrt_cand == cand)
            continue;

        /* Skip primes (p = 0^2 + p) */
        if (is_prime_big(cand))
            continue;

        /* Check if cand can be expressed as t^2 + prime for some t >= 1 */
        bool ok = true;
        /* Only t values whose square is at most cand-2 can give prime diff >= 2 */
        seq_t limit = cand > 2 ? cand - 2 : 0;
        seq_t sqrt_limit = isqrt_seq(limit);

        for (idx_t t = 1; t <= (idx_t)sqrt_limit; ++t) {
            seq_t diff = cand - (seq_t)t * t;
            if (diff >= 2 && is_prime_big(diff)) {
                ok = false;
                break;
            }
        }

        if (ok) {
            append_candidate(&cache, &cache_len, &cache_cap, cand);
        }
    }

    return cache[n - 1];
}
