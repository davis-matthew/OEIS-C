/*
 * A070099  Number of integer triangles with perimeter n and relatively prime
 *          side lengths which are acute and isosceles.
 * https://oeis.org/A070099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer isosceles acute triangles with
 *            perimeter n and coprime sides. For each n, count pairs (a,b)
 *            with 2a+b=n, a>n/4, b<2a, gcd(a,b)=1, and b^2<2a^2
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cached_limit = 0;

/* GCD for general seq_t (works for built‑in integers and arbitrary‑precision classes
   that support operator% and operator==). */
static seq_t seq_gcd(seq_t a, seq_t b) {
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute a(n) for all n in (cached_limit, limit] and store them in cache. */
static void compute_range(idx_t limit) {
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t) * (limit + 1));
        if (!cache) return;                       /* allocation failure – should never happen */
        for (idx_t i = 0; i <= limit; ++i) cache[i] = 0;
    } else if (cached_limit + 1 <= limit) {
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (limit + 1));
        if (!new_cache) return;                   /* allocation failure – should never happen */
        cache = new_cache;
        for (idx_t i = cached_limit + 1; i <= limit; ++i) cache[i] = 0;
    }

    for (idx_t n = cached_limit + 1; n <= limit; ++n) {
        uint64_t cnt = 0;
        if (n >= 3) {
            idx_t a_start = n / 4 + 1;            /* a must satisfy a > n/4 for 2a > b */
            idx_t a_end   = (n - 1) / 2;          /* a < n/2 to keep b = n-2a > 0 */
            for (idx_t a = a_start; a <= a_end; ++a) {
                idx_t b = n - 2 * a;
                /* acute condition: b^2 < 2a^2 */
                unsigned __int128 lhs = (unsigned __int128)b * b;
                unsigned __int128 rhs = (unsigned __int128)a * a * 2;
                if (lhs >= rhs) continue;
                if (seq_gcd((seq_t)a, (seq_t)b) != 1) continue;
                ++cnt;
            }
        }
        cache[n] = (seq_t)cnt;
    }
    cached_limit = limit;
}

seq_t a(idx_t n) {
    if (n <= cached_limit) return cache[n];
    compute_range(n);
    return cache[n];
}
