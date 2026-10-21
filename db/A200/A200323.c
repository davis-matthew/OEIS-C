/*
 * A200323  For each composite m = A002808(n), a(n) is the smallest number k
 *          for which the equation x^m + (x+k)^m = (x+k+1)^m (mod m) has no
 *          solution, where x = 0..m-1.
 * https://oeis.org/A200323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest k such that, for the composite m =
 *            A002808(n) the congruence x^m + (x+k)^m ≡ (x+k+1)^m (mod m) has
 *            no solution for any x∈{0,…,m−1}
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"


/* caches for composites and the computed a-values */
static idx_t *composites = NULL;
static size_t composites_cap = 0;
static size_t composites_cnt  = 0;

static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;
static size_t a_cache_cnt  = 0;

/* ensure composites array can store at least n elements */
static void grow_composites(size_t nmin)
{
    if (composites_cap < nmin) {
        size_t new_cap = composites_cap ? composites_cap * 2 : nmin;
        while (new_cap < nmin) new_cap *= 2;
        composites = (idx_t *)realloc(composites, new_cap * sizeof(idx_t));
        composites_cap = new_cap;
    }
}

/* ensure a_cache array can store at least n elements */
static void grow_a_cache(size_t nmin)
{
    if (a_cache_cap < nmin) {
        size_t new_cap = a_cache_cap ? a_cache_cap * 2 : nmin;
        while (new_cap < nmin) new_cap *= 2;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cache_cap = new_cap;
    }
}

/* precompute the smallest k for a given composite m */
static seq_t compute_a_for_m(seq_t m)
{
    idx_t m_idx = (idx_t)m;                    /* loop bounds fit in idx_t */
    seq_t *pow_base = (seq_t *)malloc(m_idx * sizeof(seq_t));
    if (!pow_base) return 0;                    /* unlikely */

    /* pre‑compute x^m (mod m) for all x */
    for (idx_t x = 0; x < m_idx; ++x) {
        pow_base[x] = mod_pow((seq_t)x, (uint64_t)m, m);
    }

    /* test k from 1 upward */
    for (idx_t k = 1; k < m_idx; ++k) {
        bool found = false;
        for (idx_t x = 0; x < m_idx; ++x) {
            idx_t t1 = x + k;
            if (t1 >= m_idx) t1 -= m_idx;
            idx_t t2 = t1 + 1;
            if (t2 >= m_idx) t2 -= m_idx;

            seq_t sum = pow_base[x] + pow_base[t1];
            if (sum >= m) sum -= m;
            if (sum == pow_base[t2]) {
                found = true;
                break;
            }
        }
        if (!found) {
            free(pow_base);
            return k;
        }
    }

    free(pow_base);
    return m;   /* should never be reached for composite m */
}

/* generate composites and a-values up to index n */
static void fill_until(idx_t n)
{
    if (composites_cnt >= (size_t)n) return;

    idx_t start = composites_cnt ? composites[composites_cnt - 1] + 1 : 4;
    while (composites_cnt < (size_t)n) {
        if (!is_prime(start)) {
            grow_composites(composites_cnt + 1);
            grow_a_cache(a_cache_cnt + 1);
            composites[composites_cnt] = start;
            a_cache[a_cache_cnt] = compute_a_for_m((seq_t)start);
            ++composites_cnt;
            ++a_cache_cnt;
        }
        ++start;
    }
}

/* public interface required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, return 0 safely */
    fill_until(n);
    return a_cache[n - 1];
}
