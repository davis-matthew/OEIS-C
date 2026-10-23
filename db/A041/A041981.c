/*
 * A041981  Denominators of continued fraction convergents to sqrt(513).
 * https://oeis.org/A041981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 27542702*a(n-16) - a(n-32) Initial values: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=17, a(5)=20, a(6)=97, a(7)=505,
 *            a(8)=1107, a(9)=6040, a(10)=25267, a(11)=31307, a(12)=181802,
 *            a(13)=213109, a(14)=394911, a(15)=608020, a(16)=27147791, …
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 41, u128 n <= 85,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Make sure cache contains all terms up to index n (inclusive).   */
static void ensure_upto(idx_t n)
{
    if (n + 1 <= cache_len)
        return;

    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = seq_realloc(cache, cache_len, new_cap);
    cache = new_cache;

    idx_t old_len = cache_len;
    cache_len = new_cap;

    /* recompute continued‑fraction convergents up to index 31 if we have not
       already computed them.  */
    if (old_len <= 31) {
        idx_t limit = (n < 31 ? n : 31);
        const uint64_t D  = 513ULL;
        const uint64_t a0 = 22ULL;        /* floor(sqrt(513)) */
        uint64_t m = 0, d = 1, a = a0;
        seq_t q_prevprev = 0, q_prev = 1;

        cache[0] = 1;                     /* q0 */
        for (idx_t k = 1; k <= limit; ++k) {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
            seq_t q_curr = (seq_t)a * q_prev + q_prevprev;
            cache[k] = q_curr;
            q_prevprev = q_prev;
            q_prev = q_curr;
        }
    }

    /* linear recurrence for indices 32 and higher */
    idx_t start = 32;
    if (old_len > start)
        start = old_len;

    for (idx_t k = start; k <= n; ++k) {
        cache[k] = (seq_t)27542702 * cache[k - 16] - cache[k - 32];
    }
}

extern seq_t a(idx_t n)
{
    ensure_upto(n);
    return cache[n];
}
