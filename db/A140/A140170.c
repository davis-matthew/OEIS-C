/*
 * A140170  Numbers n such that n=s^pi(d_1)+s^pi(d_2)+...+s^pi(d_k) where s is
 *          sum of its digits and d_1d_2...d_k is the decimal expansion of n.
 * https://oeis.org/A140170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A140170: a(n) is the nth positive integer n for which
   n = Σ_{digits d of n} (sumdigits(n))^π(d),
   where π(d) counts the primes ≤ d. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"

static const int pi[10] = {0, 0, 1, 2, 2, 3, 3, 4, 4, 4};

static seq_t *cache          = NULL;    /* cached sequence values   */
static size_t cache_cap      = 0;       /* allocated capacity       */
static size_t cache_len      = 0;       /* number of cached terms   */
static idx_t  next_candidate = 1;       /* next integer to test     */

static bool is_fixed(idx_t cand)
{
    /* Compute digit sum s */
    seq_t s = 0;
    idx_t t = cand;
    while (t > 0) {
        s += t % 10;
        t /= 10;
    }

    /* Pre‑compute powers of s up to exponent 4 */
    seq_t spow[5];
    spow[0] = 1;
    spow[1] = s;
    spow[2] = spow[1] * spow[1];
    spow[3] = spow[2] * spow[1];
    spow[4] = spow[3] * spow[1];

    /* Sum s^π(d) over digits of cand */
    seq_t rhs = 0;
    t = cand;
    while (t > 0) {
        int d = t % 10;
        rhs += spow[pi[d]];
        t /= 10;
    }

    return rhs == (seq_t)cand;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    /* Extend cache until it contains n terms */
    while (cache_len < n) {
        if (cache == NULL || cache_len + 1 > cache_cap) {
            grow_seq(&cache, &cache_cap, cache_len + 1);
        }
        if (is_fixed(next_candidate)) {
            cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate++;
    }
    return cache[n - 1];
}
