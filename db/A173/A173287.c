/*
 * A173287  Intersection of A173286 and A173269.
 * https://oeis.org/A173287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = intersection of A173286 and A173269: integers m for which
 *            2*p(p(p(m)))-3, 3*p(p(p(m)))-2, 2*p(p(m))-3 and 3*p(p(m))-2 are
 *            all prime. Offset: 1 (a(1) = 1)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   get_prime, grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/grow_seq.h"
#include "../utils/is_prime.h"
#include "../utils/get_prime.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_checked = 1;   /* next candidate to test */

/* Test whether m satisfies the four primality conditions. */
static bool good_candidate(idx_t m)
{
    idx_t p1 = get_prime(m);          /* p(m)  */
    idx_t p2 = get_prime(p1);         /* p(p(m)) */
    idx_t p3 = get_prime(p2);         /* p(p(p(m))) */

    idx_t v1 = (idx_t)((unsigned __int128)2 * p3 - 3);   /* 2*p3-3   */
    idx_t v2 = (idx_t)((unsigned __int128)3 * p3 - 2);   /* 3*p3-2   */
    if (!is_prime(v1) || !is_prime(v2))
        return false;

    idx_t v3 = (idx_t)((unsigned __int128)2 * p2 - 3);   /* 2*p2-3   */
    idx_t v4 = (idx_t)((unsigned __int128)3 * p2 - 2);   /* 3*p2-2   */
    if (!is_prime(v3) || !is_prime(v4))
        return false;

    return true;
}

/* Grow the cache so that it contains at least n terms. */
static void ensure_n(idx_t n)
{
    while (cache_len < n) {
        while (true) {
            idx_t m = last_checked++;
            if (good_candidate(m)) {
                if (cache_len == cache_cap)
                    grow_seq((seq_t **)&cache, &cache_cap, cache_len + 1);
                cache[cache_len++] = (seq_t)m;
                break;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* not defined for n=0; return 0 for safety */
    ensure_n(n);
    return cache[n - 1];
}
