/*
 * A176865  Numbers n such that n-LargestCube is prime, (LargestCube <= n).
 * https://oeis.org/A176865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer m (m ≥ 1) such that m - ⌊∛m⌋³ is a
 *            prime number
 * Range:     u8 n <= 74, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   int_cube_root, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/int_cube_root.h"     /* idx_t int_cube_root(idx_t n); */
#include "../utils/is_prime_big.h"      /* bool is_prime_big(seq_t n); */

static void ensure_cache(size_t needed, seq_t **p, size_t *cap)
{
    if (needed <= *cap)
        return;
    size_t newcap = *cap ?: 128;
    while (newcap < needed)
        newcap <<= 1;
    *p = (seq_t *)realloc(*p, newcap * sizeof(seq_t));
    *cap = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1, but avoid UB */
        return 0;

    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t  last_checked = 0;

    while (cache_size < n) {
        ++last_checked;
        idx_t c = int_cube_root(last_checked);
        seq_t cs = (seq_t)c;
        seq_t c2 = cs * cs;
        seq_t c3 = c2 * cs;          /* c³ */

        seq_t val   = (seq_t)last_checked;
        seq_t diff  = val - c3;      /* non‑negative */

        if (is_prime_big(diff)) {
            ensure_cache(cache_size + 1, &cache, &cache_cap);
            cache[cache_size++] = val;
        }
    }
    return cache[n-1];
}
