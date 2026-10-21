/*
 * A090960  Beginning with 1, least multiple of n such that every partial sum
 *          is a perfect cube.
 * https://oeis.org/A090960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest multiple of n such that the partial sums
 *            a(1)+…+a(k) are perfect cubes for all 1 ≤ k ≤ n. Implementation:
 *            keep a running value current_root (the cube root of the partial
 *            sum after the last term). For each new index k it's enough to
 *            find the smallest integer r > current_root for wh...
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   cube_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/cube_mod.h"

/* cache of computed terms: a(1) is stored at index 0 */
static seq_t *cache      = NULL;   /* a(i) for i=1.. */
static size_t cap        = 0;      /* allocated size of cache   */
static size_t len        = 0;      /* number of terms actually stored */
static seq_t current_root = 0;     /* cube root after the last term */

/* --------------------------------------------------------------- */
/*  a(idx_t n)   ------------------------------------------------ */
seq_t a(idx_t n)
{
    /* undefined for n == 0 – return 0 to avoid UB */
    if (n == 0) return 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 16;                     /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();          /* memory failure */
        cache[0] = (seq_t)1;          /* a(1) = 1 */
        current_root = (seq_t)1;      /* partial sum after first term is 1³ */
        len = 1;                      /* one term stored */
    }

    /* return cached value if already present */
    if ((size_t)n <= len) return cache[n - 1];

    /* grow the cache if necessary */
    if ((size_t)n > cap) {
        size_t newcap = cap;
        while (newcap < (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = newcap;
    }

    /* compute terms from len to n-1 (0‑based indices) */
    for (size_t i = len; i < (size_t)n; ++i) {
        idx_t k = (idx_t)(i + 1);     /* 1‑based index */

        /* target remainder: current_root³ mod k */
        seq_t target = cube_mod(current_root, k);

        /* search for the smallest r > current_root with r³ ≡ target (mod k) */
        seq_t r = current_root + 1;
        while (1) {
            if (cube_mod(r, k) == target) break;
            ++r;
        }

        /* compute a(k) = r³ – current_root³ */
        seq_t r3 = r * r * r;
        seq_t cr3 = current_root * current_root * current_root;
        cache[i] = r3 - cr3;

        /* update for next iteration */
        current_root = r;
    }

    len = (size_t)n;
    return cache[n - 1];
}
