/*
 * A084663  a(1) = 8; a(n) = a(n-1) + gcd(a(n-1), n).
 * https://oeis.org/A084663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 8; a(n) = a(n-1) + gcd(a(n-1), n) for n ≥ 2
 * Range:     u8 n <= 123, u16 n <= 26613, u32 n <= 50000, u64 n <= 50000,
 *            u128 n <= 50000, big any n
 * Validated: standard, pass, 80 terms through n = 9626 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

/* Cache for previously computed values */
static seq_t *cache = NULL;      /* 1‑based array */
static size_t cap = 0;           /* allocated capacity (index 0 unused) */
static idx_t last = 0;           /* highest index computed */

/* Expand the cache to contain values up to `need` */
static void ensure_cache(idx_t need)
{
    if (need <= last) return;

    /* Initialize if first time */
    if (cap == 0) {
        cap = 4;                        /* initial capacity */
        cache = mem_new(seq_t, cap);
        if (!cache) {                   /* out of memory – abort */
            exit(1);
        }
        /* a(1) = 8 */
        cache[1] = (seq_t)8;
        last = 1;
    }

    /* Grow array if necessary */
    while (need > last) {
        if ((size_t)last + 1 >= cap) {
            size_t newcap = cap * 2;
            seq_t *tmp = mem_resize(seq_t, cache, newcap);
            if (!tmp) exit(1);
            cache = tmp;
            cap = newcap;
        }
        /* Compute next term */
        seq_t prev = cache[last];
        seq_t g = gcd(prev, (seq_t)(last + 1));
        cache[last + 1] = prev + g;
        ++last;
    }
}

/* Exported sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined below offset, return 0 */
    ensure_cache(n);
    return cache[n];
}
