/*
 * A083018  Primes whose 10's complement is a cube.
 * https://oeis.org/A083018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-03)
 * Helpers:   ipow, ipow_i32, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes whose 10ᵈ‑complement is a perfect cube.
 * Generates the n-th prime p (1‑based) such that p = 10ᵈ – m³
 * for some integer m, where d is the number of decimal digits of p.
 * The sequence is produced lazily and cached; calls may arrive in any order.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow.h"
#include "../utils/ipow_i32.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

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
