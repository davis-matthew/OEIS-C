/*
 * A091401  Numbers n such that genus of group Gamma_0(n) is zero.
 * https://oeis.org/A091401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   cusps_prime_power, elliptic_points_order2,
 *            elliptic_points_order3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A091401.
   a(1) = 1, a(2) = 2, … 
   Each term n is the integer such that the genus of the congruence
   subgroup Γ₀(n) is zero.
   The algorithm tests the genus formula
      12 + μ - 3·e₂ - 4·e₃ - 6·c₀ = 0
   where μ = n∏_(p|n)(1+1/p),
         e₂ = number of elliptic points of order 2,
         e₃ = number of elliptic points of order 3,
         c₀ = number of cusps.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/cusps_prime_power.h"
#include "../utils/elliptic_points_order2.h"
#include "../utils/elliptic_points_order3.h"

/* Prime factorisation of an unsigned 64‑bit integer.
   The caller must provide arrays large enough for all factors (≤64). */
static void factor_u64(uint64_t n, uint64_t *pr, int *ex, size_t *cnt)
{
    size_t c = 0;
    uint64_t t = n;
    if ((t & 1ULL) == 0) {
        int e = 0;
        while ((t & 1ULL) == 0) {
            t >>= 1;
            ++e;
        }
        pr[c] = 2ULL;
        ex[c] = e;
        ++c;
    }
    for (uint64_t f = 3; f * f <= t; f += 2) {
        if (t % f == 0) {
            int e = 0;
            while (t % f == 0) {
                t /= f;
                ++e;
            }
            pr[c] = f;
            ex[c] = e;
            ++c;
        }
    }
    if (t > 1) {
        pr[c] = t;
        ex[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Computes whether the genus of Γ₀(n) equals zero. */
static bool genus_zero(uint64_t n)
{
    if (n == 1ULL) return true;

    uint64_t pr[64];
    int ex[64];
    size_t cnt;
    factor_u64(n, pr, ex, &cnt);

    /* μ = n * ∏(1 + 1/p) */
    seq_t mu = (seq_t)n;
    for (size_t i = 0; i < cnt; ++i) {
        mu = mu * ((seq_t)pr[i] + 1) / (seq_t)pr[i];
    }

    /* Elliptic points */
    seq_t e2 = elliptic_points_order2(pr, ex, cnt);
    seq_t e3 = elliptic_points_order3(pr, ex, cnt);

    /* Cusps */
    seq_t cusps = 1;
    for (size_t i = 0; i < cnt; ++i) {
        cusps *= cusps_prime_power(pr[i], ex[i]);
    }

    /* 12 + μ - 3e₂ - 4e₃ - 6c₀ */
    seq_t lhs = 12 + mu - 3 * e2 - 4 * e3 - 6 * cusps;
    return lhs == 0;
}

/* ---------- memoised sequence ------------------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t last_checked = 1;  /* we already have n=1 in cache? no */

/* Append a new term to the cache */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1 */

    /* initialise cache with first term if empty */
    if (cache_len == 0) {
        cache_append((seq_t)1);
        last_checked = 1;
    }

    /* Generate terms until we have n of them */
    while (cache_len < (size_t)n) {
        ++last_checked;
        if (genus_zero(last_checked))
            cache_append((seq_t)last_checked);
    }
    return cache[(size_t)n - 1];
}
