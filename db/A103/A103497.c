/*
 * A103497  Multiplicative suborder of 11 (mod n) = sord(11, n).
 * https://oeis.org/A103497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 545, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = minimum e≥1 such that 11^e ≡ ±1 (mod n), or 0 if no such e exists.
 * Implementation: brute‑force exponentiation up to (n‑1)/2 (as guaranteed for n>2),
 * with simple memoisation to keep repeated calls fast.
 */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* external linkage as required */
seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0 || n == 1) return 0;
    if (n == 2) return 1;

    /* static cache */
    static seq_t *cache  = NULL;       /* computed values */
    static unsigned char *seen = NULL; /* 0 = not computed, 1 = computed */
    static idx_t cache_size = 0;

    /* enlarge cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        unsigned char *tmp_seen = (unsigned char *)realloc(seen, new_size * sizeof(unsigned char));
        if (!tmp_cache || !tmp_seen) { /* allocation failure – fall back to no cache */
            cache = NULL;
            seen  = NULL;
            cache_size = 0;
        } else {
            cache     = tmp_cache;
            seen      = tmp_seen;
            for (idx_t i = cache_size; i < new_size; ++i) {
                cache[i] = 0;
                seen[i]  = 0;
            }
            cache_size = new_size;
        }
    }

    /* use cached value if available */
    if (seen && seen[n]) {
        return cache[n];
    }

    /* compute the value */
    seq_t mod  = (seq_t)n;
    seq_t cur  = 11 % mod;           /* 11^1 mod n */
    idx_t limit = (n - 1) / 2;        /* guaranteed upper bound for n>2 */
    seq_t ans = 0;

    for (idx_t e = 1; e <= limit; ++e) {
        if (cur == 1 || cur == mod - 1) {
            ans = e;
            break;
        }
        cur = (cur * (seq_t)11) % mod;
    }

    /* store in cache */
    if (seen) {
        cache[n] = ans;
        seen[n]  = 1;
    }

    return ans;
}
