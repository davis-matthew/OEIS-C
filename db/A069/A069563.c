/*
 * A069563  a(1) = 1, a(2) = 4; for n > 2, a(n) = k*a(n-1) + 1 where k is
 *          smallest number > 1 such that k*a(n-1) + 1 is a multiple of n.
 * https://oeis.org/A069563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function computes OEIS A069563:
 *
 *  a(1) = 1,  a(2) = 4.
 *  For n > 2:
 *      a(n) = k * a(n-1) + 1
 *      where k is the smallest integer > 1 such that
 *      k*a(n-1)+1 ≡ 0 (mod n).
 *  If no such k exists the sequence is undefined from that point on.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/gcd.h"            /* provided helper */

static seq_t *cache = NULL;   /* cached terms: cache[0] = a(1) */
static idx_t cache_cap = 0;   /* allocated size (1‑based) */
static idx_t cache_len = 0;   /* number of terms stored */

/* Ensure that the cache can hold at least `required` terms */
static void
ensure_cache_capacity(idx_t required)
{
    if (required <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : required;
    while (new_cap < required)
        new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!p)
        return;                /* allocation failure – keep old cache */
    cache = p;
    cache_cap = new_cap;
}

/* Extended Euclid on signed 64‑bit values, returns inverse of a mod mod */
static long long
egcd_inv(long long a, long long mod)
{
    long long b = mod, u = 1, v = 0;
    while (b != 0) {
        long long t = a / b;
        long long tmp = a - t * b;   a = b; b = tmp;
        tmp = u - t * v;             u = v; v = tmp;
    }
    /* a is gcd, u is modular inverse of original a (mod mod) */
    if (a != 1)          /* no inverse */
        return -1LL;
    if (u < 0)
        u += mod;
    return u;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* out of range, return 0 */

    /* Grow cache to n terms if necessary */
    ensure_cache_capacity(n);

    /* Compute missing terms */
    for (idx_t m = cache_len + 1; m <= n; ++m) {
        if (m == 1) {
            cache[0] = (seq_t)1;
        } else if (m == 2) {
            cache[1] = (seq_t)4;
        } else {
            seq_t prev = cache[m - 2];

            /* Compute gcd(prev, m) via prev mod m */
            idx_t prev_mod = (idx_t)(prev % (seq_t)m);
            seq_t g = gcd((seq_t)prev_mod, (seq_t)m);
            if (g != (seq_t)1) {            /* no x solving k*prev ≡ -1 (mod m) */
                cache[m - 1] = (seq_t)0;
                cache_len = m;
                break;                      /* further terms remain undefined */
            }

            long long inv = egcd_inv((long long)prev_mod, (long long)m);
            /* inv ∈ [1, m-1] because gcd(prev_mod,m)=1 */
            idx_t k0 = (idx_t)((m - inv) % m);   /* k such that k*prev ≡ -1 (mod m) */
            if (k0 <= 1)                         /* k must be > 1 */
                k0 += m;
            seq_t term = (seq_t)k0 * prev + (seq_t)1;
            cache[m - 1] = term;
        }
        cache_len = m;
    }

    return cache[n - 1];
}
