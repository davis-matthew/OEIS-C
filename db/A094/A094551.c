/*
 * A094551  Numbers k such that there are integers a < b with
 *          a+(a+1)+...+(k-1) = k+(k+1)+...+b.
 * https://oeis.org/A094551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k where k is the n‑th integer such that there exist
 *            integers a < b with a + (a+1) + … + (k-1) = k + (k+1) + … + b.
 *            Equivalently, 2·S(k‑1) = S(a‑1) + S(b) where S(x) = x(x+1)/2
 * Range:     u8 n <= 146, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

/* integer square root on unsigned __int128, result fits in uint64_t */
static inline unsigned long long isqrt128(unsigned __int128 v)
{
    unsigned long long r = (unsigned long long)sqrtl((long double)v);
    while ((unsigned __int128)r * r > v) --r;
    while ((unsigned __int128)(r + 1) * (r + 1) <= v) ++r;
    return r;
}

/* check whether a given k satisfies the property */
static bool has_k(idx_t k)
{
    if (k < 3) return false;          /* the sequence starts at 3 */

    /* triangular number T = S(k-1) */
    unsigned __int128 km1 = (unsigned __int128)k - 1;
    unsigned __int128 T  = km1 * k / 2;                /* S(k-1) */
    unsigned __int128 twoT = T << 1;                   /* 2*T */

    for (idx_t m = 0; m <= k - 2; ++m) {
        unsigned __int128 Sm = (unsigned __int128)m * (m + 1) / 2; /* S(m) */
        if (Sm > twoT) break;                                 /* safety */
        unsigned __int128 target = twoT - Sm;                 /* RHS of quadratic */

        unsigned __int128 disc = (unsigned __int128)1 + (unsigned __int128)8 * target;
        unsigned long long s = isqrt128(disc);
        if ((unsigned __int128)s * s != disc) continue;       /* not a perfect square */

        /* n = (-1 + s) / 2 must be integer */
        if ((s & 1ULL) == 0ULL) continue;
        unsigned long long n = (s - 1) >> 1;
        if ((unsigned __int128)n * (n + 1) / 2 != target) continue; /* safety check */

        if (n >= (unsigned long long)k) return true;         /* found a valid pair */
    }
    return false;
}

/* memoization of the sequence values */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static idx_t last_checked = 0;

static void ensure_cache(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 32;
    if (new_cap < need) new_cap = need;
    seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_arr) exit(1);
    seq_cache = new_arr;
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but return 0 for safety */

    ensure_cache(n);
    while (seq_len < n) {
        ++last_checked;
        if (has_k(last_checked))
            seq_cache[seq_len++] = (seq_t)last_checked;
    }
    return seq_cache[n - 1];
}
