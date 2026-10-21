/*
 * A098030  Areas of integer-sided triangles whose area equals their
 *          perimeter.
 * https://oeis.org/A098030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   computed by searching all integer‑sided triangles with area
 *            equal to perimeter
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Returns the n‑th term of A098030 (1‑based index). */
seq_t a(idx_t n)
{
    /* Build the cache once. */
    if (cache_size == 0) {
        size_t cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;              /* out of memory */
        cache_size = 0;

        /* Derive u ≤ v ≤ w from the identity */
        /*   u + v + w = p   and   u * v * w = 16(p)   where
         *   p = 2a + 2b + 2c  and   u = p – 2a  etc.   */
        for (idx_t u = 1; u <= 6; ++u) {                     /* u ≤ 6 because u * v ≤ 48 */
            idx_t max_v = 48 / u;
            if (max_v == 0) continue;
            for (idx_t v = u; v <= max_v; ++v) {
                idx_t uv = u * v;
                if (uv <= 16) continue;                    /* uv must be > 16 */
                idx_t denom = uv - 16;
                idx_t numer = 16 * (u + v);
                if (numer % denom != 0) continue;
                idx_t w = numer / denom;
                if (w < v) continue;                      /* enforce u ≤ v ≤ w */
                seq_t p = (seq_t)u + (seq_t)v + (seq_t)w;  /* perimeter */

                /* Recover side lengths */
                if (((p - (seq_t)u) & 1) || ((p - (seq_t)v) & 1) ||
                    ((p - (seq_t)w) & 1))
                    continue;                              /* sides would not be integers */

                seq_t a1 = (p - (seq_t)u) / 2;
                seq_t b1 = (p - (seq_t)v) / 2;
                seq_t c1 = (p - (seq_t)w) / 2;
                if (a1 == 0 || b1 == 0 || c1 == 0) continue;
                /* Triangle inequality check */
                if (a1 + b1 <= c1 || a1 + c1 <= b1 || b1 + c1 <= a1)
                    continue;

                /* Store unique perimeters */
                bool found = false;
                for (size_t k = 0; k < cache_size; ++k) {
                    if (cache[k] == p) { found = true; break; }
                }
                if (!found) {
                    if (cache_size == cap) {
                        cap <<= 1;
                        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                        if (!cache) return 0;
                    }
                    cache[cache_size++] = p;
                }
            }
        }

        /* Sort the cache in ascending order. */
        for (size_t i = 1; i < cache_size; ++i) {
            seq_t key = cache[i];
            size_t j = i;
            while (j > 0 && cache[j - 1] > key) {
                cache[j] = cache[j - 1];
                --j;
            }
            cache[j] = key;
        }
    }

    if (n == 0 || n > cache_size) return 0;
    return cache[n - 1];
}
