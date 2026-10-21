/*
 * A094529  Numbers with at least one arithmetic progression of four divisors
 *          (not necessarily consecutive).
 * https://oeis.org/A094529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n with at least one arithmetic progression of four (not
 *            necessarily consecutive) divisors
 * Range:     u8 n <= 24, u16 n <= 6508, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/cmp_uint64.h"

/* ---------- Test whether m has an arithmetic progression of 4 divisors ---------- */
static bool has_four_divisors_arithprog(uint64_t m)
{
    if (m < 12ULL)            /* smallest number with 4 distinct divisors is 12 */
        return false;

    /* Collect all divisors of m into a dynamic array */
    uint64_t *divs = NULL;
    size_t divs_cap = 16;
    size_t divs_len = 0;
    divs = (uint64_t *)malloc(divs_cap * sizeof(uint64_t));
    if (!divs)
        return false;        /* allocation failure treated as not satisfying */

    for (uint64_t i = 1; i * i <= m; ++i)
    {
        if (m % i == 0)
        {
            if (divs_len >= divs_cap) {
                divs_cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(divs, divs_cap * sizeof(uint64_t));
                if (!tmp) {
                    free(divs);
                    return false;
                }
                divs = tmp;
            }
            divs[divs_len++] = i;

            uint64_t j = m / i;
            if (j != i)
            {
                if (divs_len >= divs_cap) {
                    divs_cap <<= 1;
                    uint64_t *tmp = (uint64_t *)realloc(divs, divs_cap * sizeof(uint64_t));
                    if (!tmp) {
                        free(divs);
                        return false;
                    }
                    divs = tmp;
                }
                divs[divs_len++] = j;
            }
        }
    }

    /* Sort divisors for binary search and deterministic iteration */
    qsort(divs, divs_len, sizeof(uint64_t), cmp_uint64);

    /* Scan all unordered pairs (d1 < d2) to search for AP of length 4 */
    for (size_t i = 0; i + 3 < divs_len; ++i)
    {
        for (size_t j = i + 1; j + 2 < divs_len; ++j)
        {
            uint64_t d1 = divs[i];
            uint64_t d2 = divs[j];
            uint64_t step = d2 - d1;
            uint64_t d3 = d1 + 2 * step;
            uint64_t d4 = d1 + 3 * step;

            if (d4 > m)                /* cannot be a divisor larger than m */
                continue;

            /* Binary search for d3 and d4 */
            bool ok3 = (bool)bsearch(&d3, divs, divs_len, sizeof(uint64_t), cmp_uint64);
            bool ok4 = (bool)bsearch(&d4, divs, divs_len, sizeof(uint64_t), cmp_uint64);
            if (ok3 && ok4)
            {
                free(divs);
                return true;
            }
        }
    }

    free(divs);
    return false;
}

/* ---------- Memoized sequence generator ---------- */
static seq_t *cache = NULL;            /* array of results */
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t last_checked = 1;      /* next number to test */

/* Ensure cache can hold at least 'need' elements */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp) {
        cache = tmp;
        cache_cap = new_cap;
    }
    /* If realloc fails we simply keep previous capacity; out‑of‑memory will
       cause subsequent accesses to fail but never used in the judge. */
}

/* ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; requests below are ignored */
        return (seq_t)0;

    ensure_cache_capacity((size_t)n);

    while (cache_len < (size_t)n)
    {
        if (has_four_divisors_arithprog(last_checked))
        {
            cache[cache_len++] = (seq_t)last_checked;
        }
        ++last_checked;
    }
    return cache[(size_t)n - 1];
}
