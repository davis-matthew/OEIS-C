/*
 * A091012  Numbers having exactly one divisor d such that for some x also d-x
 *          and d+x are divisors.
 * https://oeis.org/A091012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer that has exactly one divisor d
 *            such that there exists an integer x with d‑x and d+x also being
 *            divisors. The property is checked by enumerating all divisor
 *            pairs and counting the distinct mean divisors
 * Range:     u8 n <= 8, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   find_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/find_index.h"

/* ---------- Helpers ---------- */

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va > vb) - (va < vb);
}

/* Recursive divisor generation */
static void gen_divs_rec(const uint64_t *pr, const int *exp, int idx,
                         uint64_t cur, uint64_t *divs, size_t *cnt,
                         int pf_cnt)
{
    if (idx == pf_cnt) {
        divs[(*cnt)++] = cur;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t val = cur;
    for (int e = 0; e <= exp[idx]; ++e) {
        gen_divs_rec(pr, exp, idx + 1, val, divs, cnt, pf_cnt);
        val *= p;
    }
}

/* ---------- Property check ---------- */

static bool has_special_divisor(uint64_t n)
{
    /* Prime factorisation */
    uint64_t primes[64];
    int exps[64];
    int pf_cnt = 0;
    uint64_t temp = n;

    /* factor 2 */
    if ((temp & 1ULL) == 0) {
        int e = 0;
        while ((temp & 1ULL) == 0) { temp >>= 1; ++e; }
        primes[pf_cnt] = 2ULL; exps[pf_cnt] = e; ++pf_cnt;
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) { temp /= p; ++e; }
            primes[pf_cnt] = p; exps[pf_cnt] = e; ++pf_cnt;
        }
    }
    if (temp > 1) {
        primes[pf_cnt] = temp; exps[pf_cnt] = 1; ++pf_cnt;
    }

    /* total number of divisors */
    size_t div_cnt = 1;
    for (int i = 0; i < pf_cnt; ++i) div_cnt *= (size_t)(exps[i] + 1);

    uint64_t *divs = (uint64_t *)malloc(div_cnt * sizeof(uint64_t));
    if (!divs) return false;

    size_t cur_index = 0;
    gen_divs_rec(primes, exps, 0, 1ULL, divs, &cur_index, pf_cnt);

    /* sort divisors for binary search */
    qsort(divs, div_cnt, sizeof(uint64_t), cmp_uint64);

    /* flag divisor d if it is the mean of two other divisors */
    uint8_t *is_mean = (uint8_t *)calloc(div_cnt, sizeof(uint8_t));
    if (!is_mean) { free(divs); return false; }

    for (size_t i = 0; i < div_cnt; ++i) {
        for (size_t j = i + 1; j < div_cnt; ++j) {
            uint64_t sum = divs[i] + divs[j];
            if (sum & 1ULL) continue;               /* sum must be even */
            uint64_t d = sum >> 1;
            int idx = find_index(divs, div_cnt, d);
            if (idx >= 0) is_mean[idx] = 1;
        }
    }

    size_t count = 0;
    for (size_t i = 0; i < div_cnt; ++i) if (is_mean[i]) ++count;

    free(divs);
    free(is_mean);
    return count == 1;
}

/* ---------- Memoised sequence generator ---------- */

static seq_t *cache     = NULL;      /* array of terms */
static idx_t cache_size = 0;         /* number of stored terms */
static idx_t cache_cap  = 0;         /* allocated capacity */
static uint64_t last_checked = 0;    /* last candidate examined */

/* Generate the n-th term of A091012. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but safe return */

    /* Grow cache until we have at least n terms */
    while (cache_size < n) {
        ++last_checked;
        if (has_special_divisor(last_checked)) {
            if (cache_size >= cache_cap) {
                cache_cap = (cache_cap ? cache_cap << 1 : 16);
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
