/*
 * A083018  Primes whose 10's complement is a cube.
 * https://oeis.org/A083018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 8, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   big_pow, floor_cube_root, is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes whose 10ᵈ‑complement is a perfect cube.
 * Generates the n-th prime p (1‑based) such that p = 10ᵈ – m³
 * for some integer m, where d is the number of decimal digits of p.
 * The sequence is produced lazily and cached; calls may arrive in any order.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/big_pow.h"
#include "../utils/floor_cube_root.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* offset is 1, n==0 unused */

    /* Cache of discovered terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;              /* number of terms stored */
    static size_t cache_cap = 0;              /* capacity of cache */
    static idx_t last_d = 0;                 /* last processed digit length */
    static seq_t pow10 = 1;                  /* 10ᵈ for current d */
    static seq_t prev_pow10 = 1;              /* 10ᵈ⁻¹ for current d */

    /* Allocate / grow cache as needed */
    if (cache_len < n && cache == NULL) {
        cache_cap = 16;
        cache = seq_realloc(cache, 0, cache_cap);
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        /* Advance to next digit length */
        ++last_d;
        prev_pow10 = pow10;
        pow10 = prev_pow10 * ((seq_t)10);     /* 10ᵈ */

        /* Compute maximum m such that m³ ≤ 9·10ᵈ⁻¹ */
        seq_t limit = 9 * prev_pow10;
        idx_t m_max = (idx_t)floor_cube_root(limit);

        /* Scan m from m_max down to 1 (p increases as m decreases) */
        for (idx_t m = m_max; m >= 1; --m) {
            seq_t mseq = (seq_t)m;
            seq_t m2   = mseq * mseq;        /* m² */
            seq_t m3   = m2 * mseq;          /* m³ */
            seq_t p    = pow10 - m3;         /* candidate prime */

            if (is_prime_big(p)) {
                /* Append to cache, growing if necessary */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    cache = seq_realloc(cache, cache_cap, new_cap);
                    cache_cap = new_cap;
                }
                cache[cache_len++] = p;
            }

            /* Avoid underflow when m==1 */
            if (m == 1) break;
        }
    }

    return cache[n - 1];
}
